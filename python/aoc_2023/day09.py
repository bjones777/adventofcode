def all_zeros(values : [int]) -> bool:
    for value in values:
        if value != 0:
            return False
    return True

def extrapulate(values : [int]) -> int:
    adj_diffs = [ values ]
    while not all_zeros(adj_diffs[-1]):
        new_values = []
        last_values = adj_diffs[-1]
        for i in range(1, len(last_values)):
            new_values.append(last_values[i] - last_values[i-1])
        adj_diffs.append(new_values)
    adj_diffs[-1].append(0)
    for i in range(1, len(adj_diffs)):
        index = len(adj_diffs) - 1 - i
        adj_diffs[index].append(adj_diffs[index+1][-1] + adj_diffs[index][-1])
    return adj_diffs[0][-1]

def part_a(lines : [str]) -> int:
    sum = 0
    for line in lines:
        values = [int(x.strip()) for x in line.split(" ") if x.strip() != ""]
        new_value = extrapulate(values)
        sum += new_value
    return sum

def part_b(lines : [str]) -> int:
    sum = 0
    for line in lines:
        values = [int(x.strip()) for x in line.split(" ") if x.strip() != ""]
        values.reverse()
        new_value = extrapulate(values)
        sum += new_value
    return sum

def main():
    lines = []
    with open("day09.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}")   
    print(f"Part B: {part_b(lines)}")   
    
if __name__ == "__main__":
    main()