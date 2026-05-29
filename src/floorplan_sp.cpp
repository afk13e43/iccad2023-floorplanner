// ICCAD 2023 CAD Contest - Problem D
// Fixed-Outline Floorplanning with Rectilinear Soft Blocks
//
// Clean Sequence-Pair + Simulated-Annealing floorplanner (rectangle stage),
// with multi-threaded parallel restarts (island model).
//
//  - Representation: Sequence Pair (posSeq, negSeq) over soft modules.
//  - Decode: connectivity-aware constructive placement. Modules are placed in
//    SP priority order; each is dropped at the legal candidate position (resting
//    against the outline / placed rects / fixed pads) that minimises added HPWL.
//    Incremental HPWL is separable in x,y, so candidates are ranked per-axis and
//    searched best-first with pruning -> coordinate-based, no pixel scan, fast.
//  - Cost: HPWL (bbox-center Manhattan x weight) + heavy penalty per module that
//    cannot be placed inside the outline.
//  - Parallelism: each thread is an independent annealer (own RNG + state); the
//    global best across all threads is reported. Restarts are embarrassingly
//    parallel, so this scales ~linearly with cores.
//
// Build:  g++ -O2 -std=c++17 -static -pthread floorplan_sp.cpp -o prog
//   (-static avoids loading a mismatched libstdc++ DLL on Windows/MinGW)
// Run:    ./prog <input> <output> [time_limit_sec] [threads]

#include <bits/stdc++.h>
using namespace std;
using Clock = chrono::steady_clock;

// ----------------------------- Data ---------------------------------------

struct Module {
    string name;
    bool   isFixed = false;
    long long reqArea = 0;   // soft only
    int    w = 0, h = 0;     // current dimensions
    int    x = 0, y = 0;     // current bottom-left position
};

struct Net  { int a, b; long long weight; };
struct Rect { int x1, y1, x2, y2; };   // [x1,x2) x [y1,y2)

// ---- Shared, read-only after parsing (safe to share across threads) -------

static int chipW = 0, chipH = 0;
static int S = 0, F = 0;                          // #soft, #fixed
static vector<Module> masterMods;                 // parsed modules (template)
static vector<Net>    nets;
static vector<Rect>   padRects;                   // fixed module obstacles
static vector<vector<pair<int,long long>>> adj;   // per-module: (neighbor, weight)

static inline double centerX(const Module& m) { return m.x + m.w / 2.0; }
static inline double centerY(const Module& m) { return m.y + m.h / 2.0; }

// Reshape soft module to a target ratio t = w/h, keeping area >= reqArea and
// aspect ratio (h/w) within [0.5, 2.0]. Pure (depends only on m).
static void reshapeTo(Module& m, double t) {
    if (t < 0.5) t = 0.5;
    if (t > 2.0) t = 2.0;
    long long a = m.reqArea;
    int wlo = (int)ceil(sqrt((double)a / 2.0));
    int whi = (int)floor(sqrt((double)a * 2.0));
    if (wlo < 1) wlo = 1;
    if (whi < wlo) whi = wlo;
    int w = (int)llround(sqrt((double)a * t));
    if (w < wlo) w = wlo;
    if (w > whi) w = whi;
    int h = (int)((a + w - 1) / w);
    if (h < 1) h = 1;
    while ((double)h / w > 2.0 && w < whi) { ++w; h = (int)((a + w - 1) / w); }
    while ((double)h / w < 0.5 && w > wlo) { --w; h = (int)((a + w - 1) / w); }
    m.w = w; m.h = h;
}

static void initShapeSquare(Module& m) { reshapeTo(m, 1.0); }

// ----------------------------- Parsing -------------------------------------

static int moduleIndex(const string& name) {
    for (int i = 0; i < (int)masterMods.size(); ++i)
        if (masterMods[i].name == name) return i;
    return -1;
}

static void parseInput(const string& file) {
    ifstream in(file);
    if (!in) { cerr << "cannot open " << file << "\n"; exit(1); }
    string tok;
    while (in >> tok) {
        if (tok == "CHIP") {
            in >> chipW >> chipH;
        } else if (tok == "SOFTMODULE") {
            in >> S;
            for (int i = 0; i < S; ++i) {
                Module m; m.isFixed = false;
                in >> m.name >> m.reqArea;
                initShapeSquare(m);
                masterMods.push_back(m);
            }
        } else if (tok == "FIXEDMODULE") {
            in >> F;
            for (int i = 0; i < F; ++i) {
                Module m; m.isFixed = true;
                in >> m.name >> m.x >> m.y >> m.w >> m.h;
                masterMods.push_back(m);
            }
        } else if (tok == "CONNECTION") {
            int k; in >> k;
            for (int i = 0; i < k; ++i) {
                string n1, n2; long long wt;
                in >> n1 >> n2 >> wt;
                int a = moduleIndex(n1), b = moduleIndex(n2);
                if (a >= 0 && b >= 0) nets.push_back({a, b, wt});
            }
        }
    }
    for (int i = 0; i < (int)masterMods.size(); ++i)
        if (masterMods[i].isFixed)
            padRects.push_back({masterMods[i].x, masterMods[i].y,
                                masterMods[i].x + masterMods[i].w,
                                masterMods[i].y + masterMods[i].h});
    adj.assign(masterMods.size(), {});
    for (const Net& n : nets) {
        adj[n.a].push_back({n.b, n.weight});
        adj[n.b].push_back({n.a, n.weight});
    }
}

// ----------------------------- Solver (per thread) -------------------------

static const double PENALTY = 1e15;   // per unplaced module

struct Best { vector<int> pos, neg, w, h; double cost = DBL_MAX; };

struct Solver {
    vector<Module> mods;                       // own working copy
    vector<int> posSeq, negSeq, posIdx, negIdx, placeOrder;
    vector<char> placedFlag;
    vector<int> candX, candY, ox, oy;          // decode scratch
    vector<double> vx, vy;
    mt19937 rng{0xC0FFEE};
    Best best;
    long long samples = 0;

    void init(unsigned seed) {
        mods = masterMods;
        rng.seed(seed);
        posSeq.resize(S); negSeq.resize(S);
        posIdx.resize(S); negIdx.resize(S);
        best.cost = DBL_MAX;
    }

    inline int    randInt(int n) { return (int)(rng() % (unsigned)n); }
    inline double randReal() { return (double)rng() / (double)mt19937::max(); }

    void rebuildIdx() {
        for (int i = 0; i < S; ++i) { posIdx[posSeq[i]] = i; negIdx[negSeq[i]] = i; }
    }

    // Modules early in BOTH sequences are anchored first (both sequences matter).
    void buildPlaceOrder() {
        placeOrder.resize(S);
        for (int i = 0; i < S; ++i) placeOrder[i] = i;
        sort(placeOrder.begin(), placeOrder.end(), [&](int a, int b){
            int ka = posIdx[a] + negIdx[a], kb = posIdx[b] + negIdx[b];
            return ka != kb ? ka < kb : a < b;
        });
    }

    // Connectivity-aware constructive decode. Returns #modules that could not be
    // placed inside the outline (0 => fully legal, overlap-free placement).
    int decode() {
        buildPlaceOrder();
        vector<Rect> obs = padRects;
        obs.reserve(padRects.size() + S);
        placedFlag.assign(mods.size(), 0);
        for (int i = S; i < (int)mods.size(); ++i) placedFlag[i] = 1;  // pads

        int unplaced = 0;
        for (int oi = 0; oi < S; ++oi) {
            int id = placeOrder[oi];
            Module& m = mods[id];

            // Candidate coords: rest left/right of and above/below each obstacle.
            candX.clear(); candY.clear();
            candX.push_back(0); candY.push_back(0);
            for (const Rect& r : obs) {
                candX.push_back(r.x2);        candX.push_back(r.x1 - m.w);
                candY.push_back(r.y2);        candY.push_back(r.y1 - m.h);
            }
            auto prep = [](vector<int>& v, int hi){
                for (int& q : v) { if (q < 0) q = 0; if (q > hi) q = hi; }
                sort(v.begin(), v.end()); v.erase(unique(v.begin(), v.end()), v.end());
            };
            prep(candX, chipW - m.w);
            prep(candY, chipH - m.h);

            int K = candX.size(), L = candY.size();
            vx.resize(K); vy.resize(L);
            for (int i = 0; i < K; ++i) {
                double cx = candX[i] + m.w / 2.0, s = 0;
                for (auto& e : adj[id]) if (placedFlag[e.first])
                    s += (double)e.second * fabs(cx - centerX(mods[e.first]));
                vx[i] = s;
            }
            for (int j = 0; j < L; ++j) {
                double cy = candY[j] + m.h / 2.0, s = 0;
                for (auto& e : adj[id]) if (placedFlag[e.first])
                    s += (double)e.second * fabs(cy - centerY(mods[e.first]));
                vy[j] = s;
            }
            ox.resize(K); oy.resize(L);
            iota(ox.begin(), ox.end(), 0); iota(oy.begin(), oy.end(), 0);
            sort(ox.begin(), ox.end(), [&](int a, int b){ return vx[a] < vx[b]; });
            sort(oy.begin(), oy.end(), [&](int a, int b){ return vy[a] < vy[b]; });
            double vyMin = L ? vy[oy[0]] : 0;

            double bestC = DBL_MAX; int bx = -1, by = -1;
            for (int ii = 0; ii < K; ++ii) {
                int i = ox[ii];
                if (vx[i] + vyMin >= bestC) break;
                for (int jj = 0; jj < L; ++jj) {
                    int j = oy[jj];
                    double c = vx[i] + vy[j];
                    if (c >= bestC) break;
                    int x = candX[i], y = candY[j];
                    bool ok = true;
                    for (const Rect& r : obs)
                        if (x < r.x2 && r.x1 < x + m.w && y < r.y2 && r.y1 < y + m.h) { ok = false; break; }
                    if (ok) { bestC = c; bx = x; by = y; break; }
                }
            }

            if (bx < 0) { m.x = 0; m.y = 0; ++unplaced; continue; }
            m.x = bx; m.y = by; placedFlag[id] = 1;
            obs.push_back({bx, by, bx + m.w, by + m.h});
        }
        return unplaced;
    }

    double hpwl() {
        double s = 0;
        for (const Net& n : nets)
            s += (double)n.weight *
                 (fabs(centerX(mods[n.a]) - centerX(mods[n.b])) +
                  fabs(centerY(mods[n.a]) - centerY(mods[n.b])));
        return s;
    }
    double cost(int unplaced) { return hpwl() + (double)unplaced * PENALTY; }

    void captureBest(double c) {
        best.cost = c; best.pos = posSeq; best.neg = negSeq;
        best.w.resize(S); best.h.resize(S);
        for (int i = 0; i < S; ++i) { best.w[i] = mods[i].w; best.h[i] = mods[i].h; }
    }
    void loadBest() {
        posSeq = best.pos; negSeq = best.neg;
        for (int i = 0; i < S; ++i) { mods[i].w = best.w[i]; mods[i].h = best.h[i]; }
        rebuildIdx();
    }

    // One annealing chain per thread, cooled on WALL-TIME so the schedule spans
    // exactly the available budget (independent of sample rate / thread count).
    // Reheat segments give exploration; the global best is kept across them.
    void anneal(Clock::time_point start, double total) {
        const double T0 = 1e7, Tmin = 1.0;
        const int segments = 4;                 // reheat this many times
        const double segLen = total / segments;

        for (int i = 0; i < S; ++i) posSeq[i] = negSeq[i] = i;
        shuffle(posSeq.begin(), posSeq.end(), rng);
        shuffle(negSeq.begin(), negSeq.end(), rng);
        for (int i = 0; i < S; ++i) reshapeTo(mods[i], 1.0);
        rebuildIdx();
        double cur = cost(decode());
        if (cur < best.cost) captureBest(cur);

        double T = T0;
        bool done = false;
        while (!done) {
            double el = chrono::duration<double>(Clock::now() - start).count();
            if (el >= total) break;
            // Temperature follows the position within the current reheat segment.
            double segFrac = fmod(el, segLen) / segLen;       // 0..1 within segment
            T = T0 * pow(Tmin / T0, segFrac);

            for (int burst = 0; burst < 256; ++burst) {       // amortise clock calls
                int type = randInt(3);
                int a = 0, b = 0, id = 0, ow = 0, oh = 0;
                if (type < 2 && S >= 2) {
                    a = randInt(S); b = randInt(S);
                    vector<int>& seq = (type == 0 ? posSeq : negSeq);
                    vector<int>& idx = (type == 0 ? posIdx : negIdx);
                    swap(seq[a], seq[b]); idx[seq[a]] = a; idx[seq[b]] = b;
                } else {
                    type = 2; id = randInt(S);
                    ow = mods[id].w; oh = mods[id].h;
                    reshapeTo(mods[id], 0.5 + randReal() * 1.5);
                }

                double nc = cost(decode());
                ++samples;
                double d = nc - cur;
                if (d < 0 || randReal() < exp(-d / T)) {
                    cur = nc;
                    if (nc < best.cost) captureBest(nc);
                } else {
                    if (type < 2) {
                        vector<int>& seq = (type == 0 ? posSeq : negSeq);
                        vector<int>& idx = (type == 0 ? posIdx : negIdx);
                        swap(seq[a], seq[b]); idx[seq[a]] = a; idx[seq[b]] = b;
                    } else { mods[id].w = ow; mods[id].h = oh; }
                }
            }
        }
    }

    bool legal() {
        for (int i = 0; i < S; ++i) {
            const Module& m = mods[i];
            if (m.x < 0 || m.y < 0 || m.x + m.w > chipW || m.y + m.h > chipH) return false;
            if ((long long)m.w * m.h < m.reqArea) return false;
            double ar = (double)m.h / (double)m.w;
            if (ar < 0.5 - 1e-9 || ar > 2.0 + 1e-9) return false;
        }
        auto overlap = [](const Module& a, const Module& b){
            return a.x < b.x + b.w && b.x < a.x + a.w &&
                   a.y < b.y + b.h && b.y < a.y + a.h;
        };
        for (int i = 0; i < S; ++i)
            for (int j = i + 1; j < (int)mods.size(); ++j)
                if (overlap(mods[i], mods[j])) return false;
        return true;
    }

    void writeOutput(const string& file) {
        double H = hpwl();
        ofstream out(file);
        out << "HPWL ";
        if (H == floor(H)) out << fixed << setprecision(0) << H << "\n";
        else               out << fixed << setprecision(1) << H << "\n";
        out << "SOFTMODULE " << S << "\n";
        for (int i = 0; i < S; ++i) {
            const Module& m = mods[i];
            out << m.name << " 4\n";
            out << m.x       << " " << m.y       << "\n";   // clockwise: BL,TL,TR,BR
            out << m.x       << " " << m.y + m.h << "\n";
            out << m.x + m.w << " " << m.y + m.h << "\n";
            out << m.x + m.w << " " << m.y       << "\n";
        }
    }
};

// ----------------------------- main ----------------------------------------

int main(int argc, char** argv) {
    if (argc < 3) { cerr << "usage: " << argv[0] << " <input> <output> [sec] [threads]\n"; return 1; }
    parseInput(argv[1]);

    double tl = (argc >= 4) ? atof(argv[3]) : 60.0;
    unsigned nthreads = (argc >= 5) ? (unsigned)atoi(argv[4])
                                    : max(1u, thread::hardware_concurrency());
    if (nthreads < 1) nthreads = 1;

    vector<Solver> solvers(nthreads);
    unsigned baseSeed = (unsigned)Clock::now().time_since_epoch().count();
    for (unsigned t = 0; t < nthreads; ++t)
        solvers[t].init(baseSeed ^ (0x9E3779B9u * (t + 1)));

    auto start = Clock::now();
    vector<thread> pool;
    for (unsigned t = 0; t < nthreads; ++t)
        pool.emplace_back([&, t]{ solvers[t].anneal(start, tl); });
    for (auto& th : pool) th.join();

    int bi = 0;
    for (unsigned t = 1; t < nthreads; ++t)
        if (solvers[t].best.cost < solvers[bi].best.cost) bi = t;

    solvers[bi].loadBest();
    solvers[bi].decode();
    solvers[bi].writeOutput(argv[2]);

    long long total = 0;
    for (auto& s : solvers) total += s.samples;
    cerr << "HPWL=" << fixed << setprecision(1) << solvers[bi].hpwl()
         << "  threads=" << nthreads
         << "  total_samples=" << total
         << "  legal=" << (solvers[bi].legal() ? "YES" : "NO") << "\n";
    return 0;
}
