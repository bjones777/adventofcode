import re

def get_match_value(line : str) -> int:
    colon_index = line.index(":")
    starting_point = colon_index + 1
    expected_versus_actual = line[starting_point:].split("|")

    expected_set = set([int(num.strip()) for num in expected_versus_actual[0].split(" ") if num.strip() != ""])
    actual_set = set([int(num.strip()) for num in expected_versus_actual[1].split(" ") if num.strip() != ""])

    num_matched = len(expected_set.intersection(actual_set))
    if num_matched == 0:
        return 0
    return 2 ** (num_matched - 1)

def get_num_matches(line : str) -> int:
    colon_index = line.index(":")
    starting_point = colon_index + 1
    expected_versus_actual = line[starting_point:].split("|")

    expected_set = set([int(num.strip()) for num in expected_versus_actual[0].split(" ") if num.strip() != ""])
    actual_set = set([int(num.strip()) for num in expected_versus_actual[1].split(" ") if num.strip() != ""])

    num_matched = len(expected_set.intersection(actual_set))
    if num_matched == 0:
        return 0
    return num_matched


def part_a(lines : [str]) -> int:
    
    sum = 0
    for line in lines:
        sum += get_match_value(line)
    return sum

def part_b(lines : [str]) -> int:
    counts = []
    for line in lines:
        counts.append(1)
    
    sum = 0
    for i in range(len(lines)):
        sum += counts[i]
        if counts[i] == 0:
            break
        num_matches = get_num_matches(lines[i])
        for j in range(i+1, min(len(lines), i+num_matches+1)):
            counts[j] += counts[i] 
    
    return sum

def main():
    lines = []
    with open("day04.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}")
    print(f"Part B: {part_b(lines)}")
    
if __name__ == "__main__":
    main()