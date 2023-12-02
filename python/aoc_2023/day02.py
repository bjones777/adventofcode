import re

def is_ball_possible_1(count: int,ball : str) -> bool:
    if ball == "red":
        return count <= 12
    elif ball == "green":
        return count <= 13
    elif ball == "blue":
        return count <= 14
    else:
        raise ValueError(f"Unknown ball color: {ball}")

def is_part_possible_1(part : str) -> bool:
    pattern = r"(\d+) (\w+)"
    balls = part.split(", ")
    for ball in balls:
        match = re.search(pattern, ball)
        count = int(match.group(1))
        ball = match.group(2)
        if not is_ball_possible_1(count, ball):
            return False
    return True

def is_possible_1(line : str) -> bool:
    colon_index = line.index(":")
    starting_point = colon_index + 1

    game_parts = line[starting_point:].split(";")

    game_parts = [part.strip() for part in game_parts]

    for part in game_parts:
        if not is_part_possible_1(part):
            return False
    return True

def part_a(lines : [str]) -> int:
    sum = 0
    pattern = r"Game (\d+):"

    for line in lines:
        if is_possible_1(line):
            match = re.search(pattern, line)        
            sum += int(match.group(1))
    return sum

def get_part_power(part : str) -> (int,int,int):
    pattern = r"(\d+) (\w+)"
    balls = part.split(", ")
    power = (0,0,0)
    for ball in balls:
        match = re.search(pattern, ball)
        count = int(match.group(1))
        ball = match.group(2)
        if ball == "red":
            power = (max(power[0],count), power[1], power[2])
        elif ball == "green":
            power = (power[0], max(power[1],count), power[2])
        elif ball == "blue":
            power = (power[0], power[1], max(power[2],count))
        else:
            raise ValueError(f"Unknown ball color: {ball}")
    return power

def get_power_line(line : str) -> int:
    colon_index = line.index(":")
    starting_point = colon_index + 1

    game_parts = line[starting_point:].split(";")

    game_parts = [part.strip() for part in game_parts]

    power = (0,0,0)
    for part in game_parts:
        part_power = get_part_power(part)
        power = (max(power[0],part_power[0]), max(power[1],part_power[1]), max(power[2],part_power[2]))
    return power[0] * power[1] * power[2]
   

def part_b(lines : [str]) -> int:
    sum = 0
    
    for line in lines:
        sum += get_power_line(line)
    return sum

def main():
    lines = []
    with open("day02.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}")
    print(f"Part B: {part_b(lines)}")
    
if __name__ == "__main__":
    main()