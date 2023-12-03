def is_part_number(start_index : int, end_index : int, lines : [str], line_index: int) -> bool:
    if start_index == end_index:
        return False
    for row in range(line_index-1, line_index+2):
        if row < 0 or row >= len(lines):
            continue
        line = lines[row]
        for col in range(start_index-1, end_index+1):
            if col < 0 or col >= len(line):
                continue
            if line[col] != "." and not line[col].isdigit():
                return True
    return False

def part_a(lines : [str]) -> int:
    sum = 0
    for row in range(len(lines)):
        line = lines[row]
        cur_num = ""
        for i in range(len(line)):
            if line[i].isdigit():
                cur_num += line[i]
            else:
                start_index = i - len(cur_num)
                end_index = i
                if is_part_number(start_index, end_index, lines, row):
                    sum += int(cur_num)
                cur_num = ""
        start_index = i - len(cur_num)
        end_index = i
        if is_part_number(start_index, end_index, lines, row):
            sum += int(cur_num)
       
    return sum

def get_gear_locations(start_index : int, end_index : int, lines : [str], line_index: int) -> ([(int,int)]):
    gear_locs = []
    
    if start_index == end_index:
        return gear_locs
    
    for row in range(line_index-1, line_index+2):
        if row < 0 or row >= len(lines):
            continue
        line = lines[row]
        for col in range(start_index-1, end_index+1):
            if col < 0 or col >= len(line):
                continue
            if line[col] == "*":
                gear_locs.append((row,col))
    return gear_locs

def part_b(lines : [str]) -> int:
    gears_to_numbers = {}
    
    for row in range(len(lines)):
        line = lines[row]
        cur_num = ""
        for i in range(len(line)):
            if line[i].isdigit():
                cur_num += line[i]
            else:
                start_index = i - len(cur_num)
                end_index = i
                for gear_loc in get_gear_locations(start_index, end_index, lines, row):
                    if gear_loc not in gears_to_numbers:
                        gears_to_numbers[gear_loc] = [int(cur_num)]
                    else:
                        gears_to_numbers[gear_loc].append(int(cur_num))
                cur_num = ""
        start_index = i - len(cur_num)
        end_index = i
        for gear_loc in get_gear_locations(start_index, end_index, lines, row):
            if gear_loc not in gears_to_numbers:
                gears_to_numbers[gear_loc] = [int(cur_num)]
            else:
                gears_to_numbers[gear_loc].append(int(cur_num))

    sum = 0
    for gear_loc in gears_to_numbers:
        gear_nums = gears_to_numbers[gear_loc]
        if len(gear_nums) == 2:
            sum += gear_nums[0] * gear_nums[1]
    return sum

def main():
    lines = []
    with open("day03.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}")
    print(f"Part B: {part_b(lines)}")
    
if __name__ == "__main__":
    main()