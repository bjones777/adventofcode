def find_horizontal_reflection(lines : [str]) -> int:
    for i in range(1, len(lines)):
        up_index = i - 1
        down_index = i 
        is_possible_ref = True
        while up_index >= 0 and down_index < len(lines):
            if lines[up_index] != lines[down_index]:
                is_possible_ref = False
                break
            up_index -= 1
            down_index += 1
        if is_possible_ref:
            return i
        
    return -1

def find_horizontal_reflection_with_smudge(lines : [str]) -> int:
    for i in range(1, len(lines)):
        up_index = i - 1
        down_index = i 
        num_diffs = 0
        while up_index >= 0 and down_index < len(lines):
            for j in range(0, len(lines[0])):
                if lines[up_index][j] != lines[down_index][j]:
                    num_diffs += 1
                    if num_diffs > 1:
                        break
            up_index -= 1
            down_index += 1
        if num_diffs == 1:
            return i
        
    return -1

def find_vertical_reflection(lines : [str]) -> int:
    for i in range(1, len(lines[0])):
        left_index = i - 1
        right_index = i 
        is_possible_ref = True
        while left_index >= 0 and right_index < len(lines[0]):
            for j in range(0, len(lines)):
                if lines[j][left_index] != lines[j][right_index]:
                    is_possible_ref = False
                    break
            if not is_possible_ref:
                break;
            left_index -= 1
            right_index += 1
        if is_possible_ref:
            return i
    raise Exception("No reflection found")

def find_vertical_reflection_with_smudge(lines : [str]) -> int:
    for i in range(1, len(lines[0])):
        left_index = i - 1
        right_index = i 
        num_diffs = 0
        while left_index >= 0 and right_index < len(lines[0]):
            for j in range(0, len(lines)):
                if lines[j][left_index] != lines[j][right_index]:
                    num_diffs += 1
                    if num_diffs > 1:
                        break
            if num_diffs > 1:
                break
            left_index -= 1
            right_index += 1
        if num_diffs == 1:
            return i
    raise Exception("No reflection found")


def part_a(lines : [str]):
    start_line_index = 0
    
    summary = 0

    while True:
        while start_line_index < len(lines) and lines[start_line_index] == "":
            start_line_index += 1
        if start_line_index >= len(lines):
            break
        end_line_index = start_line_index + 1
        while end_line_index < len(lines) and lines[end_line_index] != "":
            end_line_index += 1

        reflection = find_horizontal_reflection(lines[start_line_index:end_line_index])
        if reflection != -1:
            summary += 100 * reflection
        else:
            reflection = find_vertical_reflection(lines[start_line_index:end_line_index])
            summary += reflection

        start_line_index = end_line_index + 1

    return summary

def part_b(lines : [str]):
    start_line_index = 0
    
    summary = 0

    while True:
        while start_line_index < len(lines) and lines[start_line_index] == "":
            start_line_index += 1
        if start_line_index >= len(lines):
            break
        end_line_index = start_line_index + 1
        while end_line_index < len(lines) and lines[end_line_index] != "":
            end_line_index += 1

        reflection = find_horizontal_reflection_with_smudge(lines[start_line_index:end_line_index])
        if reflection != -1:
            summary += 100 * reflection
        else:
            reflection = find_vertical_reflection_with_smudge(lines[start_line_index:end_line_index])
            summary += reflection

        start_line_index = end_line_index + 1

    return summary
    


def main():
    lines = []
    with open("day13.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}") 
    print(f"Part B: {part_b(lines)}")   
    
    
if __name__ == "__main__":
    main()