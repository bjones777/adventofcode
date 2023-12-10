from queue import Queue

def get_tile_type(i: int, j: int, dict : dict) -> str:
    if (i,j) in dict:
        return dict[(i,j)]
    else:
        return "."
    
def goes_down(i: int, j: int, dict : dict) -> bool:
    tile_type = get_tile_type(i, j, dict)
    return tile_type == "|" or tile_type == "F" or tile_type == "7"

def goes_left(i: int, j: int, dict : dict) -> bool:
    tile_type = get_tile_type(i, j, dict)
    return tile_type == "J" or tile_type == "7" or tile_type == "-"

def goes_right(i: int, j: int, dict : dict) -> bool:
    tile_type = get_tile_type(i, j, dict)
    return tile_type == "F" or tile_type == "L" or tile_type == "-"

def goes_up(i: int, j: int, dict : dict) -> bool:
    tile_type = get_tile_type(i, j, dict)
    return tile_type == "|" or tile_type == "L" or tile_type == "J"

    
def identify_start_tile(i: int, j: int, dict : dict) -> str:
    if goes_down(i-1, j, dict) and goes_up(i+1, j, dict):
        return "|"
    if goes_down(i-1, j, dict)  and goes_left(i, j+1, dict):
        return "L"
    if goes_up(i-1, j, dict)  and goes_right(i, j-1, dict) :
        return "J"
    if goes_up(i+1, j, dict)  and goes_left(i, j+1, dict):
        return "F"
    if goes_down(i+1, j, dict)  and goes_right(i, j-1, dict) :
        return "7"
    if goes_right(i, j-1, dict) and goes_left(i, j-1, dict) :
        return "-"
    raise Exception("Unable to identify start tile")

def get_next_location(i: int, j: int, di: int, dj: int, dict : dict) -> (int, int, int, int):
    title_type = get_tile_type(i, j, dict)
    if title_type == "|":
        return (i+di, j+dj, di, dj)
    if title_type == "-":
        return (i+di, j+dj, di, dj)
    if title_type == "L":
        if di == 0:
            return (i-1, j, -1, 0)
        else:
            return (i, j+1, 0, 1)
    if title_type == "J":
        if di == 0:
            return (i-1, j, -1, 0)
        else:
            return (i, j-1, 0, -1)
    if title_type == "F":
        if di == 0:
            return (i+1, j, 1, 0)
        else:
            return (i, j+1, 0, 1)
    if title_type == "7":
        if di == 0:
            return (i+1, j, 1, 0)
        else:
            return (i, j-1, 0, -1)
    raise Exception(f"Unknown tile type {title_type}")

def part_a(lines : [str]) -> int:
    dict = {}
    start_loc = None
    for i, line in enumerate(lines):
        for j, ch in enumerate(line):
            dict[(i,j)] = ch
            if ch == "S":
                start_loc = (i,j)
    if start_loc == None:
        raise Exception("No start location found")
    to_visit = Queue()
    start_tile_type = identify_start_tile(start_loc[0], start_loc[1], dict)
    if start_tile_type == "|":
        to_visit.put((start_loc[0]-1, start_loc[1], -1, 0))
        to_visit.put((start_loc[0]+1, start_loc[1],  1, 0))
    elif start_tile_type == "-":
        to_visit.put((start_loc[0], start_loc[1]-1,  0, -1))
        to_visit.put((start_loc[0], start_loc[1]+1,  0, 1))
    elif start_tile_type == "L":
        to_visit.put((start_loc[0]-1, start_loc[1],  -1, 0))
        to_visit.put((start_loc[0], start_loc[1]+1,  0, 1))
    elif start_tile_type == "J":
        to_visit.put((start_loc[0]-1, start_loc[1],  -1, 0))
        to_visit.put((start_loc[0], start_loc[1]-1,  0, -1))
    elif start_tile_type == "F":
        to_visit.put((start_loc[0]+1, start_loc[1], +1, 0))
        to_visit.put((start_loc[0], start_loc[1]+1,  0, 1))
    elif start_tile_type == "7":
        to_visit.put((start_loc[0]+1, start_loc[1], +1, 0))
        to_visit.put((start_loc[0], start_loc[1]-1,  0, -1))
    else:
        raise Exception(f"Unknown start tile type {start_tile_type}")
    
    num_steps = 1
    while True:
        i, j, di, dj = to_visit.get()
        k, l, dk, dl = to_visit.get()
            
        if i == k and j == l:
            return num_steps
        
        to_visit.put(get_next_location(i, j, di, dj, dict))
        to_visit.put(get_next_location(k, l, dk, dl, dict))
        num_steps += 1
    
def solve_on_big_map(dict, loop_tiles, min_x, max_x, min_y, max_y):
    big_map = {}
    for i in range(min_x, max_x+1):
        start_bi = i * 3
        for j in range(min_y, max_y+1):
            start_bj = j * 3
            if (i,j) in loop_tiles:
                tile_type = get_tile_type(i, j, dict)
                if tile_type == "S":
                    tile_type = identify_start_tile(i, j, dict)
                if tile_type == "|":
                    big_map[(start_bi+0, start_bj+1)] = True
                    big_map[(start_bi+1, start_bj+1)] = True
                    big_map[(start_bi+2, start_bj+1)] = True
                elif tile_type == "-":
                    big_map[(start_bi+1, start_bj+0)] = True
                    big_map[(start_bi+1, start_bj+1)] = True
                    big_map[(start_bi+1, start_bj+2)] = True
                elif tile_type == "L":
                    big_map[(start_bi+0, start_bj+1)] = True
                    big_map[(start_bi+1, start_bj+1)] = True
                    big_map[(start_bi+1, start_bj+2)] = True
                elif tile_type == "F":
                    big_map[(start_bi+1, start_bj+1)] = True
                    big_map[(start_bi+2, start_bj+1)] = True
                    big_map[(start_bi+1, start_bj+2)] = True
                elif tile_type == "J":
                    big_map[(start_bi+0, start_bj+1)] = True
                    big_map[(start_bi+1, start_bj+1)] = True
                    big_map[(start_bi+1, start_bj+0)] = True
                elif tile_type == "7":
                    big_map[(start_bi+1, start_bj+1)] = True
                    big_map[(start_bi+1, start_bj+0)] = True
                    big_map[(start_bi+2, start_bj+1)] = True
                else:
                    raise Exception(f"Unknown loop tile type {tile_type}")
    to_visit = Queue()
    bmin_x = min_x * 3
    bmax_x = max_x * 3 + 2
    bmin_y = min_y * 3
    bmax_y = max_y * 3 + 2
    to_visit.put((bmin_x, bmin_y))
    visited_tiles = {}
    while not to_visit.empty():
        i, j = to_visit.get()
        if i < bmin_x or i > bmax_x or j < bmin_y or j > bmax_y:
            continue
        if (i,j) in visited_tiles:
            continue
        visited_tiles[(i,j)] = True
        if (i,j) in big_map:
            continue
        big_map[(i,j)] = True
        if (i-1,j) not in visited_tiles:
            to_visit.put((i-1,j))
        if (i+1,j) not in visited_tiles:
            to_visit.put((i+1,j))
        if (i,j-1) not in visited_tiles:
            to_visit.put((i,j-1))
        if (i,j+1) not in visited_tiles:
            to_visit.put((i,j+1))

    num_interior_tiles = 0
    for i in range(min_x, max_x+1):
        start_bi = i * 3
        for j in range(min_y, max_y+1):
            start_bj = j * 3
            is_interior = True
            for k in range(0,3):
                for l in range(0,3):
                    if (start_bi+k, start_bj+l) in big_map:
                        is_interior = False
                        break
            if is_interior:
                num_interior_tiles += 1
    return num_interior_tiles
    

            
def part_b(lines : [str]) -> int:
    dict = {}
    start_loc = None
    for i, line in enumerate(lines):
        for j, ch in enumerate(line):
            dict[(i,j)] = ch
            if ch == "S":
                start_loc = (i,j)
    if start_loc == None:
        raise Exception("No start location found")
    to_visit = Queue()
    start_tile_type = identify_start_tile(start_loc[0], start_loc[1], dict)
    if start_tile_type == "|":
        to_visit.put((start_loc[0]-1, start_loc[1], -1, 0))
        to_visit.put((start_loc[0]+1, start_loc[1],  1, 0))
    elif start_tile_type == "-":
        to_visit.put((start_loc[0], start_loc[1]-1,  0, -1))
        to_visit.put((start_loc[0], start_loc[1]+1,  0, 1))
    elif start_tile_type == "L":
        to_visit.put((start_loc[0]-1, start_loc[1],  -1, 0))
        to_visit.put((start_loc[0], start_loc[1]+1,  0, 1))
    elif start_tile_type == "J":
        to_visit.put((start_loc[0]-1, start_loc[1],  -1, 0))
        to_visit.put((start_loc[0], start_loc[1]-1,  0, -1))
    elif start_tile_type == "F":
        to_visit.put((start_loc[0]+1, start_loc[1], +1, 0))
        to_visit.put((start_loc[0], start_loc[1]+1,  0, 1))
    elif start_tile_type == "7":
        to_visit.put((start_loc[0]+1, start_loc[1], +1, 0))
        to_visit.put((start_loc[0], start_loc[1]-1,  0, -1))
    else:
        raise Exception(f"Unknown start tile type {start_tile_type}")
    
    min_x = start_loc[0]
    max_x = start_loc[0]
    min_y = start_loc[1]
    max_y = start_loc[1]

    loop_tiles = {}
    loop_tiles[(start_loc[0], start_loc[1])] = True

    while True:
        i, j, di, dj = to_visit.get()
        k, l, dk, dl = to_visit.get()

        min_x = min(min_x, i, k)
        max_x = max(max_x, i, k)
        min_y = min(min_y, j, l)
        max_y = max(max_y, j, l)

        loop_tiles[(i,j)] = True
        loop_tiles[(k,l)] = True
            
        if i == k and j == l:
            break
        
        to_visit.put(get_next_location(i, j, di, dj, dict))
        to_visit.put(get_next_location(k, l, dk, dl, dict))

    return solve_on_big_map(dict, loop_tiles, min_x, max_x, min_y, max_y)


def main():
    lines = []
    with open("day10.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}")   
    print(f"Part B: {part_b(lines)}")
    
    
if __name__ == "__main__":
    main()