from queue import Queue

def enqueue_if_not_visited(to_visit: Queue, will_visit: set, location: (int, int), direction: (int, int), m: int, n : int) -> None:
    if location[0] < 0 or location[0] >= m or location[1] < 0 or location[1] >= n:
        return
    key = (location[0], location[1], direction[0], direction[1])
    if key not in will_visit:
        to_visit.put((location, direction))
        will_visit.add((location[0], location[1], direction[0], direction[1]))


def do_simulation(lines: [str], location: (int,int), direction: (int,int)) -> int:
    m = len(lines)
    n = len(lines[0])

    to_visit = Queue()
    will_visit = set()
    enqueue_if_not_visited(to_visit, will_visit, location, direction, m, n)

    while not to_visit.empty():
        location, direction = to_visit.get()
        ch = lines[location[0]][location[1]]
        if ch == ".":
            new_location = (location[0] + direction[0], location[1] + direction[1])
            enqueue_if_not_visited(to_visit, will_visit, new_location, direction, m, n)
        elif ch == "/":
            if direction[0] == 0:
                if direction[1] == 1:
                    enqueue_if_not_visited(to_visit, will_visit, (location[0]-1, location[1]), (-1, 0), m, n)
                else:
                    enqueue_if_not_visited(to_visit, will_visit, (location[0]+1, location[1]), (1, 0), m, n)
            else:
                if direction[0] == 1:
                    enqueue_if_not_visited(to_visit, will_visit, (location[0], location[1]-1), (0, -1), m, n)
                else:
                    enqueue_if_not_visited(to_visit, will_visit, (location[0], location[1]+1), (0, 1), m, n)
        elif ch == "\\":
            if direction[0] == 0:
                if direction[1] == 1:
                    enqueue_if_not_visited(to_visit, will_visit, (location[0]+1, location[1]), (1, 0), m, n)
                else:
                    enqueue_if_not_visited(to_visit, will_visit, (location[0]-1, location[1]), (-1, 0), m, n)
            else:
                if direction[0] == 1:
                    enqueue_if_not_visited(to_visit, will_visit, (location[0], location[1]+1), (0, 1), m, n)
                else:
                    enqueue_if_not_visited(to_visit, will_visit, (location[0], location[1]-1), (0, -1), m, n)
        elif ch == "|":
            if direction[1] != 0:
                enqueue_if_not_visited(to_visit, will_visit, (location[0]-1, location[1]), (-1, 0), m, n)
                enqueue_if_not_visited(to_visit, will_visit, (location[0]+1, location[1]), (1, 0), m, n)
            else:
                new_location = (location[0] + direction[0], location[1] + direction[1])
                enqueue_if_not_visited(to_visit, will_visit, new_location, direction, m, n)
        elif ch == "-":
            if direction[0] != 0:
                enqueue_if_not_visited(to_visit, will_visit, (location[0], location[1]-1), (0, -1), m, n)
                enqueue_if_not_visited(to_visit, will_visit, (location[0], location[1]+1), (0, 1), m, n)
            else:
                new_location = (location[0] + direction[0], location[1] + direction[1])
                enqueue_if_not_visited(to_visit, will_visit, new_location, direction, m, n)
    
    energized = set()
    for key in will_visit:
        energized.add((key[0], key[1]))
    return len(energized)  



def part_a(lines: [str]) -> int:
    return do_simulation(lines, (0,0), (0,1))
   
    
def part_b(lines: [str]) -> int:
    max_energized = 0
    m = len(lines)
    n = len(lines[0])
    for i in range(0, m):
        max_energized = max(max_energized, do_simulation(lines, (i,0), (0,1)))
        max_energized = max(max_energized, do_simulation(lines, (i,n-1), (0,-1)))
    for i in range(0, n):
        max_energized = max(max_energized, do_simulation(lines, (0,i), (1,0)))
        max_energized = max(max_energized, do_simulation(lines, (m-1,i), (-1,0)))
    return max_energized


def main():
    lines = []
    with open("day16.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}") 
    print(f"Part B: {part_b(lines)}") 

if __name__ == "__main__":
    main()
