def part_a(lines : [str]) -> int:
    rock_locations = set()

    heights = []
    for _ in range(0, len(lines[0])):
        heights.append(-1)
    for row, line in enumerate(lines):
        for col in range(0, len(line)):
            if line[col] == '#':
                heights[col] = row 
            if line[col] == 'O':
                rock_locations.add((heights[col] + 1, col ))
                heights[col] += 1

    load = 0
    for row, _ in rock_locations:
        load += len(lines) - row
    return load

def do_cycle(lines : [str], rock_locations_list : [(int,int)], dir : int) -> [(int,int)]:
    rock_locations = set(rock_locations_list)
    heights = []
    m = len(lines)
    n = len(lines[0])
    if dir == 0:
        for _ in range(0, n):
            heights.append(-1)
    elif dir == 1:
        for _ in range(0, m):
            heights.append(-1)
    elif dir == 2:
        for _ in range(0, n):
            heights.append(m)    
    else:
        for _ in range(0, m):
            heights.append(n)

    new_rock_locations = set()
    if dir == 0:
        for row in range(0, m):
            for col in range(0, n):
                if lines[row][col] == '#':
                    heights[col] = row 
                if (row, col) in rock_locations:
                    new_rock_locations.add((heights[col] + 1, col ))
                    heights[col] += 1
    elif dir == 2:        
        for ri in range(0, m):
            row = m - 1 - ri
            for col in range(0, n):
                if lines[row][col] == '#':
                    heights[col] = row 
                if (row, col) in rock_locations:
                    new_rock_locations.add((heights[col] - 1, col ))
                    heights[col] -= 1
    elif dir == 1:        
        for col in range(0, n):
            for row in range(0, m):
                if lines[row][col] == '#':
                    heights[row] = col
                if (row, col) in rock_locations:
                    new_rock_locations.add((row, heights[row] + 1 ))
                    heights[row] += 1
    else:
        for ci in range(0, n):
            col = n - 1 - ci
            for row in range(0, m):
                if lines[row][col] == '#':
                    heights[row] = col
                if (row, col) in rock_locations:
                    new_rock_locations.add((row, heights[row] - 1 ))
                    heights[row] -= 1
    return list(new_rock_locations)


def calc_load(rock_locations : [(int,int)], m : int) -> int:
    load = 0
    for row, _ in rock_locations:
        load += m - row
    return load

def print_grid(lines : [str], rock_locations : [(int,int)]):
    m = len(lines)
    n = len(lines[0])
    for row in range(0, m):
        the_line = []
        for col in range(0, n):
            if (row, col) in rock_locations:
                the_line.append('O')
            elif lines[row][col] == '#':
                the_line.append('#')
            else:
                the_line.append('.')
        print(''.join(the_line))
    print()
    
    

def part_b(lines : [str]) -> int:
    rock_locations = []

    m = len(lines)
    n = len(lines[0])

    for r in range(0, m):
        for c in range(0, n):
            if lines[r][c] == 'O':
                rock_locations.append((r, c))
    
    rock_locations.sort()
    rock_to_cycle = {}
    rock_to_cycle[tuple(rock_locations)] = 0

    cycle_to_rock = {}
    cycle_to_rock[0] = rock_locations
    
    for i in range(0, 1000000000):
        for j in range(0, 4):
            rock_locations = do_cycle(lines, rock_locations, j)
        rock_locations.sort()
        trc = tuple(rock_locations)
        if trc in rock_to_cycle:
            prev_cycle = rock_to_cycle[trc]
            current_cycle = i + 1
            needed_cycle = ((1000000000 - prev_cycle) % (current_cycle - prev_cycle)) + prev_cycle
            return calc_load(cycle_to_rock[needed_cycle], m)
        rock_to_cycle[tuple(rock_locations)] = i+1
        cycle_to_rock[i+1] = rock_locations

    return calc_load(rock_locations, m)
    


def main():
    lines = []
    with open("day14.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}") 
    print(f"Part B: {part_b(lines)}")
    
    
    
if __name__ == "__main__":
    main()