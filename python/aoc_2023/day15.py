def do_hash(str : str) -> int:
    h = 0
    for c in str:
        h = ((h+ord(c)) * 17) % 256
    return h

def part_a(lines : [str]) -> int:
    parts = lines[0].split(',')
    result = sum([do_hash(p) for p in parts])
    return result

def parse_part(part : str) -> (str, str, int):
    if part.endswith('-'):
        return (part[0:len(part)-1], '-', 0)
    
    pieces = part.split('=')
    return (pieces[0], "=", int(pieces[1]))

def part_b(lines : [str]) -> int:
    boxes = []
    box_counts = []
    for i in range(0, 256):
        boxes.append({})
        box_counts.append(0)
    parts = lines[0].split(',')
    for part in parts:
        label, op, lens = parse_part(part)
        box = do_hash(label)
        if op == '=':
            if label in boxes[box]:
                old_tuple = boxes[box][label]
                new_tuple = (old_tuple[0], lens)
                if old_tuple[0] == -1:
                    new_tuple = (box_counts[box], lens)    
                    box_counts[box] += 1
                boxes[box][label] = new_tuple
            else:
                boxes[box][label] = (box_counts[box], lens)
                box_counts[box] += 1
        else:
            if label in boxes[box]:
                old_tuple = boxes[box][label]
                new_tuple = (-1, lens)
                boxes[box][label] = new_tuple
            
    
    sum = 0
    for i in range(0, 256):
        lenses = []
        for x in boxes[i].values():
            lenses.append(x)
        lenses = [ x for x in lenses if x[0] != -1 ]
        lenses.sort()
        for j in range(0, len(lenses)):
            sum += (i+1) * (j+1) * lenses[j][1] 
    return sum
        

def main():
    lines = []
    with open("day15.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}") 
    print(f"Part B: {part_b(lines)}") 
    

    

if __name__ == "__main__":
    main()

