import re

def part_a(lines : [str]) -> int:
    lr = lines[0]

    pattern = r"(\w{3}) = \((\w{3}), (\w{3})\)"
    lookup = {}
    for line in lines[2:]:
        match = re.search(pattern, line)
        lookup[match.group(1)] = (match.group(2), match.group(3))

    num_steps = 0
    current_location = "AAA"
    while current_location != "ZZZ":
        dir = lr[num_steps % len(lr)]
        if dir == "L":
            current_location = lookup[current_location][0]
        else:
            current_location = lookup[current_location][1]
        num_steps += 1
    return num_steps

def find_path_length(current_location : str, lookup : dict, lr : str) -> int:
    num_steps = 0
    while not current_location.endswith("Z"):
        dir = lr[num_steps % len(lr)]
        if dir == "L":
            current_location = lookup[current_location][0]
        else:
            current_location = lookup[current_location][1]
        num_steps += 1
    return num_steps

def gcd(a : int, b : int) -> int:
    if b == 0:
        return a
    return gcd(b, a % b)

# This function is a lie, and not what I used to solve part b.  It is the code I wished I had written to solve part b.
# The puzzle input makes several simplifying assumptions I was unware of when I tried to solve part b.
# I originally wrote the general solution using dynamic programming to skip a large number of steps, but it was too slow.
# I eventually did some anaylsis and determined that each of them followed a formula of arrive at z by a + b * i, where a, b, and i is an integer.
# But i can vary.  I also determined that the paths don't cross, and a single "a" start only cycles to a single "z".
# This lead to the solution in part_c, which is faster and what I used to solve part b.  But is still pretty slow.  What I failed to 
# notice was that a was a multiple of b, so a reduced to 0, which means solving is just a matter of finding the least common multiple
# of all the cycles.  This is what part_b does.  But, yeah, the puzzle input had to be very carefully crafted to make this work.
def part_b(lines : [str]) -> int:
    lr = lines[0]

    current_locations = []
    pattern = r"(\w{3}) = \((\w{3}), (\w{3})\)"
    lookup = {}
    for line in lines[2:]:
        match = re.search(pattern, line)
        lookup[match.group(1)] = (match.group(2), match.group(3))
        if match.group(1).endswith("A"):
            current_locations.append(match.group(1))

    lengths = [find_path_length(current_location, lookup, lr) for current_location in current_locations]
    lcm = lengths[0]
    for i in range(1, len(lengths)):
        lcm = (lcm * lengths[i]) // gcd(lcm, lengths[i])

    return lcm

def part_c() -> int:
    eq = [(42818,21409)
          ,(36314,18157)
          ,(39566,19783)
          ,(33062,16531)
          ,(38482,19241)
          ,(28726,14363)]
    
    best_match = 0
    for i in range(0, 10000000000):
        steps = eq[0][0]  + eq[0][1] * i
        matched = 0
        works = True
        for j in range(1, len(eq)):
            if (steps - eq[j][0]) % eq[j][1] != 0:
                works = False
                break
            else:
                matched += 1
        if matched > best_match:
            best_match = matched
            print(f"Best match: {best_match} at {steps}")
        if works:
            return steps
    return -1

def main():
    lines = []
    with open("day08.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}")   
    print(f"Part b: {part_b(lines)}")   
    #print(f"Part c: {part_c()}")   
    
if __name__ == "__main__":
    main()