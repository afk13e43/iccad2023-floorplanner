// Independent legality + HPWL checker for ICCAD 2023 Problem D.
// Shares no code with the solver. Build: g++ -O2 -std=c++17 checker.cpp -o checker
// Run: ./checker caseNN-input.txt outNN.txt
#include <bits/stdc++.h>
using namespace std;

struct Box { double x1,y1,x2,y2; };

int main(int argc, char** argv){
    if(argc<3){ cerr<<"usage: checker <input> <output>\n"; return 2; }
    // ---- input ----
    ifstream in(argv[1]); string t;
    long long cw=0,ch=0;
    map<string,long long> softArea;
    map<string,Box> box;             // all modules (soft filled from output, fixed here)
    vector<tuple<string,string,long long>> nets;
    while(in>>t){
        if(t=="CHIP"){ in>>cw>>ch; }
        else if(t=="SOFTMODULE"){ int n; in>>n; for(int i=0;i<n;i++){ string nm; long long a; in>>nm>>a; softArea[nm]=a; } }
        else if(t=="FIXEDMODULE"){ int n; in>>n; for(int i=0;i<n;i++){ string nm; double x,y,w,h; in>>nm>>x>>y>>w>>h; box[nm]={x,y,x+w,y+h}; } }
        else if(t=="CONNECTION"){ int n; in>>n; for(int i=0;i<n;i++){ string a,b; long long w; in>>a>>b>>w; nets.push_back({a,b,w}); } }
    }
    // ---- output ----
    ifstream out(argv[2]);
    string kw; double fileH; out>>kw>>fileH;            // HPWL <val>
    string sm; int ns; out>>sm>>ns;                      // SOFTMODULE <n>
    map<string,vector<pair<long long,long long>>> poly;
    for(int i=0;i<ns;i++){
        string nm; int c; out>>nm>>c;
        vector<pair<long long,long long>> pts(c);
        for(int j=0;j<c;j++) out>>pts[j].first>>pts[j].second;
        poly[nm]=pts;
    }
    // ---- checks ----
    vector<string> errs;
    auto bb=[&](const vector<pair<long long,long long>>&p){
        double x1=1e18,y1=1e18,x2=-1e18,y2=-1e18;
        for(auto&q:p){ x1=min(x1,(double)q.first); y1=min(y1,(double)q.second);
                       x2=max(x2,(double)q.first); y2=max(y2,(double)q.second); }
        return Box{x1,y1,x2,y2};
    };
    auto shoelace=[&](const vector<pair<long long,long long>>&p){
        double a=0; int n=p.size();
        for(int k=0;k<n;k++){ auto&u=p[k]; auto&v=p[(k+1)%n];
            a+=(double)u.first*v.second-(double)v.first*u.second; }
        return fabs(a)/2.0;
    };
    for(auto&kv:softArea){
        const string& nm=kv.first; long long area=kv.second;
        if(!poly.count(nm)){ errs.push_back(nm+": missing in output"); continue; }
        auto& p=poly[nm]; Box b=bb(p); box[nm]=b;
        double w=b.x2-b.x1, h=b.y2-b.y1;
        if(b.x1<-1e-9||b.y1<-1e-9||b.x2>cw+1e-9||b.y2>ch+1e-9) errs.push_back(nm+": outside chip");
        double pa=shoelace(p);
        if(pa+1e-6<area) errs.push_back(nm+": polygon area "+to_string((long long)pa)+" < required "+to_string(area));
        if(w<=0||h<=0){ errs.push_back(nm+": degenerate bbox"); continue; }
        double ar=h/w;
        if(ar<0.5-1e-9||ar>2.0+1e-9) errs.push_back(nm+": aspect "+to_string(ar)+" out of [0.5,2]");
        double rr=pa/(w*h);
        if(rr<0.8-1e-9) errs.push_back(nm+": rect-ratio "+to_string(rr)+" < 0.8");
    }
    // overlaps among all boxes where at least one is soft
    vector<string> names; for(auto&kv:box) names.push_back(kv.first);
    auto ov=[&](const Box&a,const Box&b){ return a.x1<b.x2-1e-9&&b.x1<a.x2-1e-9&&a.y1<b.y2-1e-9&&b.y1<a.y2-1e-9; };
    for(size_t i=0;i<names.size();++i) for(size_t j=i+1;j<names.size();++j){
        bool s=softArea.count(names[i])||softArea.count(names[j]);
        if(s && ov(box[names[i]],box[names[j]])) errs.push_back("OVERLAP "+names[i]+" & "+names[j]);
    }
    // recompute HPWL (bbox centers)
    double H=0;
    for(auto&[a,b,w]:nets){ if(box.count(a)&&box.count(b)){
        Box&A=box[a]; Box&B=box[b];
        double ax=(A.x1+A.x2)/2, ay=(A.y1+A.y2)/2, bx=(B.x1+B.x2)/2, by=(B.y1+B.y2)/2;
        H+=(double)w*(fabs(ax-bx)+fabs(ay-by)); } }
    bool legal=errs.empty();
    bool match=fabs(H-fileH)<1.0;
    cout<<argv[2]<<": legal="<<(legal?"YES":"NO")
        <<"  HPWL_file="<<fixed<<setprecision(1)<<fileH
        <<"  HPWL_recomputed="<<H<<"  match="<<(match?"YES":"NO")<<"\n";
    for(size_t i=0;i<errs.size()&&i<25;++i) cout<<"   ! "<<errs[i]<<"\n";
    return legal?0:1;
}
