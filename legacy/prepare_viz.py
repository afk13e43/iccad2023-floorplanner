import sys

def convert():
    modules = []
    name_to_id = {}
    connections = []
    
    soft_count = 0
    fixed_count = 0
    
    # 1. Parse Input
    with open('case06-input.txt', 'r') as f:
        lines = f.read().split()
        
    idx = 0
    it = iter(lines)
    
    try:
        while True:
            token = next(it)
            if token == 'CHIP':
                chip_w = int(next(it))
                chip_h = int(next(it))
            elif token == 'SOFTMODULE':
                soft_count = int(next(it))
                for i in range(soft_count):
                    name = next(it)
                    area = int(next(it))
                    modules.append({'name': name, 'type': 'soft', 'id': len(modules)})
                    name_to_id[name] = len(modules) - 1
            elif token == 'FIXEDMODULE':
                fixed_count = int(next(it))
                for i in range(fixed_count):
                    name = next(it)
                    x = int(next(it))
                    y = int(next(it))
                    w = int(next(it))
                    h = int(next(it))
                    modules.append({'name': name, 'type': 'fixed', 'x': x, 'y': y, 'w': w, 'h': h, 'id': len(modules)})
                    name_to_id[name] = len(modules) - 1
            elif token == 'CONNECTION':
                conn_count = int(next(it))
                for i in range(conn_count):
                    n1 = next(it)
                    n2 = next(it)
                    w = int(next(it))
                    connections.append((n1, n2, w))
    except StopIteration:
        pass

    # 2. Parse Output (Soft Module placements)
    with open('case06-output.txt', 'r') as f:
        # Skip HPWL and SOFTMODULE check strictly based on file content
        content = f.read().split()
        
    it = iter(content)
    try:
        while True:
            t = next(it)
            if t == 'SOFTMODULE':
                continue
            if t == 'HPWL':
                next(it) # skip value
                continue
            
            # Assuming Name X Y W H
            name = t
            x = int(next(it))
            y = int(next(it))
            w = int(next(it))
            h = int(next(it))
            
            if name in name_to_id:
                mid = name_to_id[name]
                modules[mid]['x'] = x
                modules[mid]['y'] = y
                modules[mid]['w'] = w
                modules[mid]['h'] = h
    except StopIteration:
        pass

    # 3. Write coor.txt
    with open('coor.txt', 'w') as f:
        f.write(f"0 {soft_count} 0\n")
        for m in modules:
            if 'x' in m: # Should be all
                f.write(f"mod 4\n")
                x, y, w, h = m['x'], m['y'], m['w'], m['h']
                f.write(f"{x} {y}\n")
                f.write(f"{x+w} {y}\n")
                f.write(f"{x+w} {y+h}\n")
                f.write(f"{x} {y+h}\n")
            else:
                # Should not happen if output is complete
                print(f"Warning: Module {m['name']} has no coordinates")
                f.write(f"mod 4\n0 0\n0 0\n0 0\n0 0\n")

    # 4. Write cnt.txt
    with open('cnt.txt', 'w') as f:
        for n1, n2, w in connections:
            if n1 in name_to_id and n2 in name_to_id:
                idx1 = name_to_id[n1] + 1
                idx2 = name_to_id[n2] + 1
                f.write(f"{idx1} {idx2} {w}\n")

if __name__ == '__main__':
    convert()
