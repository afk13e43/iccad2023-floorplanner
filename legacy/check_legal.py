#!/usr/bin/env python3
# Independent legality + HPWL checker for ICCAD 2023 Problem D outputs.
# Usage: python check_legal.py caseNN-input.txt outNN.txt
import sys

def read_input(path):
    toks = open(path).read().split()
    i = 0
    chip = soft = {}
    out = {'soft': {}, 'fixed': {}, 'nets': []}
    while i < len(toks):
        t = toks[i]
        if t == 'CHIP':
            out['chip'] = (int(toks[i+1]), int(toks[i+2])); i += 3
        elif t == 'SOFTMODULE':
            n = int(toks[i+1]); i += 2
            for _ in range(n):
                out['soft'][toks[i]] = int(toks[i+1]); i += 2
        elif t == 'FIXEDMODULE':
            n = int(toks[i+1]); i += 2
            for _ in range(n):
                out['fixed'][toks[i]] = (int(toks[i+1]), int(toks[i+2]),
                                         int(toks[i+3]), int(toks[i+4])); i += 5
        elif t == 'CONNECTION':
            n = int(toks[i+1]); i += 2
            for _ in range(n):
                out['nets'].append((toks[i], toks[i+1], int(toks[i+2]))); i += 3
        else:
            i += 1
    return out

def read_output(path):
    toks = open(path).read().split()
    hpwl = float(toks[1])
    # find SOFTMODULE
    i = 2
    while toks[i] != 'SOFTMODULE': i += 1
    n = int(toks[i+1]); i += 2
    polys = {}
    for _ in range(n):
        name = toks[i]; c = int(toks[i+1]); i += 2
        pts = []
        for _ in range(c):
            pts.append((int(toks[i]), int(toks[i+1]))); i += 2
        polys[name] = pts
    return hpwl, polys

def bbox(pts):
    xs = [p[0] for p in pts]; ys = [p[1] for p in pts]
    return min(xs), min(ys), max(xs), max(ys)

def poly_area(pts):  # shoelace
    a = 0
    for k in range(len(pts)):
        x1,y1 = pts[k]; x2,y2 = pts[(k+1)%len(pts)]
        a += x1*y2 - x2*y1
    return abs(a)/2.0

def overlap(b1, b2):  # rect bbox overlap (area>0)
    return b1[0] < b2[2] and b2[0] < b1[2] and b1[1] < b2[3] and b2[1] < b1[3]

def main():
    inp, outp = sys.argv[1], sys.argv[2]
    I = read_input(inp); hpwl, polys = read_output(outp)
    cw, ch = I['chip']
    errs = []
    boxes = {}
    for name, area in I['soft'].items():
        if name not in polys: errs.append(f"{name}: missing in output"); continue
        pts = polys[name]
        x1,y1,x2,y2 = bbox(pts); boxes[name] = (x1,y1,x2,y2)
        w, h = x2-x1, y2-y1
        if x1 < 0 or y1 < 0 or x2 > cw or y2 > ch:
            errs.append(f"{name}: outside chip ({x1},{y1},{x2},{y2}) chip={cw}x{ch}")
        pa = poly_area(pts)
        if pa + 1e-6 < area:
            errs.append(f"{name}: area {pa} < required {area}")
        if w == 0 or h == 0:
            errs.append(f"{name}: degenerate"); continue
        ar = h / w
        if ar < 0.5 - 1e-9 or ar > 2.0 + 1e-9:
            errs.append(f"{name}: aspect ratio {ar:.4f} out of [0.5,2]")
        rect_ratio = pa / (w*h)
        if rect_ratio < 0.8 - 1e-9:
            errs.append(f"{name}: rect-ratio {rect_ratio:.4f} < 0.8")
    # fixed boxes
    for name,(x,y,w,h) in I['fixed'].items():
        boxes[name] = (x,y,x+w,y+h)
    # overlaps among ALL placed boxes (soft+fixed)
    names = list(boxes.keys())
    for a in range(len(names)):
        for b in range(a+1, len(names)):
            if names[a] in I['soft'] or names[b] in I['soft']:
                if overlap(boxes[names[a]], boxes[names[b]]):
                    errs.append(f"OVERLAP {names[a]} & {names[b]}")
    # recompute HPWL
    def cen(n):
        x1,y1,x2,y2 = boxes[n]; return ((x1+x2)/2.0,(y1+y2)/2.0)
    H = 0.0
    for a,b,wt in I['nets']:
        if a in boxes and b in boxes:
            ca, cb = cen(a), cen(b)
            H += wt*(abs(ca[0]-cb[0])+abs(ca[1]-cb[1]))
    ok = (len(errs)==0)
    hdiff = abs(H-hpwl)
    print(f"{outp}: legal={'YES' if ok else 'NO'}  HPWL_file={hpwl}  HPWL_recomputed={H}  match={'YES' if hdiff<1.0 else 'NO('+str(hdiff)+')'}")
    for e in errs[:20]: print("   !", e)

if __name__ == '__main__':
    main()
