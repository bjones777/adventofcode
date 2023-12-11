from bisect import bisect_left

def parts_common(lines: [str], factor: int) -> int:
    locations = []
    for i in range(len(lines)):
        for j in range(len(lines[i])):
            if lines[i][j] == "#":
                locations.append((i, j))
    
    empty_rows = []
    for i in range(len(lines)):
        if all(lines[i][j] == "." for j in range(len(lines[i]))):
            empty_rows.append(i)
    empty_cols = []
    for i in range(len(lines[0])):
        if all(lines[j][i] == "." for j in range(len(lines))):
            empty_cols.append(i)

    new_locations = []
    for (i, j) in locations:
        expanded_rows = bisect_left(empty_rows,i) * (factor-1)
        expanded_cols = bisect_left(empty_cols,j) * (factor-1)
        new_locations.append((i + expanded_rows, j + expanded_cols))

    sum = 0
    for i in range(0,len(new_locations)-1):
        for j in range(i+1,len(new_locations)):
            sum += abs(new_locations[i][0] - new_locations[j][0]) + abs(new_locations[i][1] - new_locations[j][1])
    return sum

def part_a(lines: [str]) -> int:
    return parts_common(lines, 2)
    
def part_b(lines: [str]) -> int:
    return parts_common(lines, 1000000)


def main():
    lines = []
    with open("day11.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}")   
    print(f"Part B: {part_b(lines)}")   
    
    
if __name__ == "__main__":
    main()