from queue import PriorityQueue
from enum import IntEnum, auto

class Direction(IntEnum):
    UP = 0
    DOWN = 1
    LEFT = 2
    RIGHT = 3

def get_score(location: (int, int), m: int, n: int, cost_so_far: int) -> int:
    return (m - 1 - location[0]) + (n - 1 - location[1]) + cost_so_far

def is_best_so_far(location: (int,int), cost_so_far: int, num_straight : int, direction: Direction, best_scores: dict) -> bool:
    key = (location,num_straight,direction)
    if key not in best_scores:
        best_scores[key] = cost_so_far
        return True
    if best_scores[key] <= cost_so_far:
        return False
    best_scores[key] = cost_so_far
    return True

def get_new_location(location: (int,int), direction: Direction) -> (int,int):
    if direction == Direction.UP:
        return (location[0]-1, location[1])
    elif direction == Direction.DOWN:
        return (location[0]+1, location[1])
    elif direction == Direction.LEFT:
        return (location[0], location[1]-1)
    else:
        return (location[0], location[1]+1)
    
def get_turn_left_direction(direction: Direction) -> Direction:
    if direction == Direction.UP:
        return Direction.LEFT
    elif direction == Direction.DOWN:
        return Direction.RIGHT
    elif direction == Direction.LEFT:
        return Direction.DOWN
    else:
        return Direction.UP
    
def get_turn_right_direction(direction: Direction) -> Direction:
    if direction == Direction.UP:
        return Direction.RIGHT
    elif direction == Direction.DOWN:
        return Direction.LEFT
    elif direction == Direction.LEFT:
        return Direction.UP
    else:
        return Direction.DOWN
    
def is_location_valid(location: (int,int), m: int, n: int) -> bool:
    return location[0] >= 0 and location[0] < m and location[1] >= 0 and location[1] < n

def part_a(lines: [str]) -> int:
    m = len(lines)
    n = len(lines[0])
    to_visit = PriorityQueue()
    to_visit.put((get_score((0,0),m,n,0), (0, 1), 0, 1, Direction.RIGHT))
    to_visit.put((get_score((0,0),m,n,0), (1, 0), 0, 1, Direction.DOWN))
    best_scores = {}

    while to_visit:
        _, location, cost_so_far, num_straight, direction = to_visit.get()
        ch = lines[location[0]][location[1]]
        cost_so_far += int(ch)
        if location == (m-1, n-1):
            return cost_so_far
        if not is_best_so_far(location, cost_so_far, num_straight, direction, best_scores):
            continue
        if num_straight < 3:
            new_location = get_new_location(location, direction)
            if is_location_valid(new_location, m, n):
                to_visit.put((get_score(new_location,m,n,cost_so_far), new_location, cost_so_far, num_straight+1, direction))
        new_location = get_new_location(location, get_turn_left_direction(direction))
        if is_location_valid(new_location, m, n):
            to_visit.put((get_score(new_location,m,n,cost_so_far), new_location, cost_so_far, 1, get_turn_left_direction(direction)))
        new_location = get_new_location(location, get_turn_right_direction(direction))
        if is_location_valid(new_location, m, n):
            to_visit.put((get_score(new_location,m,n,cost_so_far), new_location, cost_so_far, 1, get_turn_right_direction(direction)))
    raise Exception("No path found")  

def part_b(lines: [str]) -> int:
    m = len(lines)
    n = len(lines[0])
    to_visit = PriorityQueue()
    to_visit.put((0, (0, 1), 0, 1, Direction.RIGHT))
    to_visit.put((0, (1, 0), 0, 1, Direction.DOWN))
    best_scores = {}

    while to_visit:
        _, location, cost_so_far, num_straight, direction = to_visit.get()
        ch = lines[location[0]][location[1]]
        cost_so_far += int(ch)
        if location == (m-1, n-1) and num_straight >= 4:
            return cost_so_far
        if not is_best_so_far(location, cost_so_far, num_straight, direction, best_scores):
            continue
        if num_straight < 10:
            new_location = get_new_location(location, direction)
            if is_location_valid(new_location, m, n):
                to_visit.put((cost_so_far, new_location, cost_so_far, num_straight+1, direction))
        if num_straight >= 4:
            new_location = get_new_location(location, get_turn_left_direction(direction))
            if is_location_valid(new_location, m, n):
                to_visit.put((cost_so_far, new_location, cost_so_far, 1, get_turn_left_direction(direction)))
            new_location = get_new_location(location, get_turn_right_direction(direction))
            if is_location_valid(new_location, m, n):
                to_visit.put((cost_so_far, new_location, cost_so_far, 1, get_turn_right_direction(direction)))
    raise Exception("No path found")

def main():
    lines = []
    with open("day17.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}") 
    print(f"Part B: {part_b(lines)}") 

if __name__ == "__main__":
    main()