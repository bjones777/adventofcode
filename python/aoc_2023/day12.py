def determine_possibilities(springs: str, code: [int], si: int, ci : int, prev_code: bool, memo ) -> int:
    if (si, ci, prev_code) in memo:
        return memo[(si, ci, prev_code)]
    if si == len(springs) and ci == len(code):
        #print(output)
        return 1
    if si == len(springs):
        return 0
    if springs[si] == ".":
        result = determine_possibilities(springs, code, si+1, ci, False, memo)
        memo[(si, ci, prev_code)] = result
        return result
    if springs[si] == "#":
        result = 0
        if prev_code:
            result = 0
        elif ci >= len(code):
            result = 0
        elif si + code[ci] > len(springs):
            result = 0
        else:
            is_ok = True
            for i in range(si, si + code[ci]):
                if springs[i] == ".":
                    is_ok = False
                    break
            if is_ok:
                result = determine_possibilities(springs, code, si + code[ci], ci + 1, True, memo)
        memo[(si, ci, prev_code)] = result
        return result
    sum = 0
    sum += determine_possibilities(springs, code, si+1, ci, False, memo)
    if ci < len(code) and si + code[ci] <= len(springs) and not prev_code:
        is_ok = True
        for i in range(si, si + code[ci]):
            if springs[i] == ".":
                is_ok = False
                break
        if is_ok:
            sum += determine_possibilities(springs, code, si + code[ci], ci + 1, True, memo)
    memo[(si, ci, prev_code)] = sum
    return sum


def get_num_possibilites(line: str) -> int:
    parts = line.split(" ")
    springs = parts[0]
    code = [int(part) for part in parts[1].split(",")]
    amount = determine_possibilities(springs, code, 0, 0, False, {})
    #print(f"springs: {springs}, code: {code}, amount: {amount}")
    return amount

def get_num_possibilites2(line: str) -> int:
    memo = {}
    parts = line.split(" ")
    springs = parts[0] 
    new_springs = ""
    for i in range(0, 5):
        if i != 0:
            new_springs += "?"
        new_springs += springs
    springs = new_springs
    code = [int(part) for part in parts[1].split(",")]
    new_code = []
    for _ in range(0, 5):
        for c in code:
            new_code.append(c)
    code = new_code
    amount = determine_possibilities(springs, code, 0, 0, False, memo)
    #print(f"springs: {springs}, code: {code}, amount: {amount}")
    return amount


def part_a(lines: [str]) -> int:
    sum = 0
    for line in lines:
        sum += get_num_possibilites(line)
    return sum

def part_b(lines: [str]) -> int:
    sum = 0
    for line in lines:
        sum += get_num_possibilites2(line)
    return sum
    
def main():
    lines = []
    with open("day12.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}")   
    print(f"Part B: {part_b(lines)}")  
    
    
if __name__ == "__main__":
    main()