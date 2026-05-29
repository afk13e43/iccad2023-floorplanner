#!/usr/bin/env python3
"""Visualize an ICCAD-2023 Problem D floorplan result as a PNG.

Reads a test-case input (chip outline, fixed pads, connections, soft areas) and
a solver output (soft-module polygons) and renders:
  * the chip outline,
  * soft modules  (blue, filled, labelled)  -- supports rectilinear polygons,
  * fixed modules / pads (red, filled, labelled),
  * connections   (grey lines, thickness ~ net weight),
  * HPWL and legality summary in the title.

Usage:
    python visualize.py <input.txt> <output.txt> [out.png]

Example:
    python tools/visualize.py testcases/case06-input.txt results/out06.txt result_case6.png
"""
import sys
import matplotlib
matplotlib.use("Agg")               # headless: write file, no display needed
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon


def read_input(path):
    toks = open(path, encoding="utf-8").read().split()
    i, n = 0, len(toks)
    data = {"chip": (0, 0), "soft": {}, "fixed": {}, "nets": []}
    while i < n:
        t = toks[i]
        if t == "CHIP":
            data["chip"] = (int(toks[i + 1]), int(toks[i + 2])); i += 3
        elif t == "SOFTMODULE":
            k = int(toks[i + 1]); i += 2
            for _ in range(k):
                data["soft"][toks[i]] = int(toks[i + 1]); i += 2
        elif t == "FIXEDMODULE":
            k = int(toks[i + 1]); i += 2
            for _ in range(k):
                data["fixed"][toks[i]] = (int(toks[i + 1]), int(toks[i + 2]),
                                          int(toks[i + 3]), int(toks[i + 4])); i += 5
        elif t == "CONNECTION":
            k = int(toks[i + 1]); i += 2
            for _ in range(k):
                data["nets"].append((toks[i], toks[i + 1], int(toks[i + 2]))); i += 3
        else:
            i += 1
    return data


def read_output(path):
    toks = open(path, encoding="utf-8").read().split()
    hpwl = float(toks[1])
    i = 2
    while i < len(toks) and toks[i] != "SOFTMODULE":
        i += 1
    k = int(toks[i + 1]); i += 2
    polys = {}
    for _ in range(k):
        name = toks[i]; c = int(toks[i + 1]); i += 2
        pts = [(int(toks[i + 2 * j]), int(toks[i + 2 * j + 1])) for j in range(c)]
        i += 2 * c
        polys[name] = pts
    return hpwl, polys


def centroid_bbox(pts):
    xs = [p[0] for p in pts]; ys = [p[1] for p in pts]
    return (min(xs) + max(xs)) / 2.0, (min(ys) + max(ys)) / 2.0


def main():
    if len(sys.argv) < 3:
        print(__doc__); sys.exit(1)
    inp, outp = sys.argv[1], sys.argv[2]
    png = sys.argv[3] if len(sys.argv) >= 4 else "result.png"

    D = read_input(inp)
    hpwl, polys = read_output(outp)
    cw, ch = D["chip"]

    fig, ax = plt.subplots(figsize=(10, 10 * ch / cw if cw else 10), dpi=150)
    ax.set_xlim(0, cw); ax.set_ylim(0, ch); ax.set_aspect("equal")
    # chip outline
    ax.add_patch(Polygon([(0, 0), (cw, 0), (cw, ch), (0, ch)],
                         closed=True, fill=False, edgecolor="black", lw=2))

    centers = {}

    # soft modules (blue)
    for name in D["soft"]:
        if name not in polys:
            continue
        pts = polys[name]
        ax.add_patch(Polygon(pts, closed=True, facecolor="#4c8bf5",
                             edgecolor="#16387a", alpha=0.55, lw=1))
        cx, cy = centroid_bbox(pts); centers[name] = (cx, cy)
        ax.text(cx, cy, name, ha="center", va="center", fontsize=7, color="white")

    # fixed modules / pads (red)
    for name, (x, y, w, h) in D["fixed"].items():
        ax.add_patch(Polygon([(x, y), (x + w, y), (x + w, y + h), (x, y + h)],
                             closed=True, facecolor="#e2503c",
                             edgecolor="#7a1d12", alpha=0.7, lw=1))
        centers[name] = (x + w / 2.0, y + h / 2.0)
        ax.text(x + w / 2.0, y + h / 2.0, name, ha="center", va="center",
                fontsize=6, color="white")

    # connections (grey, thickness ~ weight)
    if D["nets"]:
        wmax = max(w for _, _, w in D["nets"])
        for a, b, w in D["nets"]:
            if a in centers and b in centers:
                (x1, y1), (x2, y2) = centers[a], centers[b]
                ax.plot([x1, x2], [y1, y2], color="black",
                        alpha=0.25, lw=0.3 + 2.5 * w / wmax, zorder=0)

    ax.set_title(f"{outp}\nchip {cw}x{ch}   HPWL = {hpwl:,.1f}", fontsize=10)
    fig.tight_layout()
    fig.savefig(png)
    print(f"saved {png}")


if __name__ == "__main__":
    main()
