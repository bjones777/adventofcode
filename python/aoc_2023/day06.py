def get_values(line : str) -> [int]:
    colon_index = line.index(":")
    starting_point = colon_index + 1

    val_strs = line[starting_point:]
    return [int(v.strip()) for v in val_strs.split(" ") if v.strip() != ""]

def get_value(line : str) -> int:
    colon_index = line.index(":")
    starting_point = colon_index + 1

    val_strs = line[starting_point:]
    val_strs = [v.strip() for v in val_strs.split(" ") if v.strip() != ""]
    return int("".join(val_strs))

def get_distance_for_time(time : int, hold: int) -> int:
    if hold >= time:
        return 0
    travel_time = time - hold
    return travel_time * hold

def get_count_for_time(time : int, distance: int) -> int:
    lb = 0
    ub = time // 2 + 1
    while lb < ub:
        mid = (lb + ub) // 2
        dist = get_distance_for_time(time, mid)
        if dist <= distance:
            lb = mid + 1
        else:
            ub = mid
    dist_from_mid =  time / 2  - lb
    return int(round(2*dist_from_mid)) + 1

def part_a(lines : [str]) -> int:
    times = get_values(lines[0])
    distances = get_values(lines[1])

    result = 1
    for i in range(0, len(times)):
        count = 0
        for hold in range(0, times[i]):
             dist = get_distance_for_time(times[i], hold)
             if dist > distances[i]:
                 count += 1
        result *= count
    return result



def part_b(lines : [str]) -> int:
    time = get_value(lines[0])
    distance = get_value(lines[1])
    return get_count_for_time(time, distance)

def main():
    lines = []
    with open("day06.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}")    
    print(f"Part B: {part_b(lines)}")    
    
if __name__ == "__main__":
    main()