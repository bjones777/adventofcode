
def part_a(lines: [str]) -> int: 
    sum = 0
   
    for line in lines:
        first_index = len(line)
        first_val = 0

        last_index = -1
        last_val = 0
        for i in range(len(line)):
            if line[i].isdigit():
                 if i < first_index:
                    first_index = i
                    first_val = int(line[i])
                    
                 if i > last_index:
                    last_index = i
                    last_val = int(line[i])

        val = first_val * 10  + last_val             
        sum += val 
    
    return sum

def part_b(lines: [str]) -> int:
    num_strs = [ "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]
    sum = 0

    for line in lines:
        first_index = len(line)
        first_val = 0

        last_index = -1
        last_val = 0

        for i in range(len(line)):
            if line[i].isdigit():
                 if i < first_index:
                    first_index = i
                    first_val = int(line[i])
                 if i > last_index:
                    last_index = i
                    last_val = int(line[i])
                   

        
        for i in range(len(num_strs)):
            num_str = num_strs[i]
            index = line.find(num_str)
            if index != -1 and index < first_index:
                first_index = index
                first_val = i + 1
        
        
        for i in range(len(num_strs)):
            num_str = num_strs[i]
            index = line.rfind(num_str)
            if index != -1 and index > last_index:
                last_index = index
                last_val = i + 1

        
        
        val = first_val * 10  + last_val             
        #print(f"{val}")
        sum += val 
        
        
    return sum

def main():
    lines = []
    with open("day01.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}")
    print(f"Part B: {part_b(lines)}")
    

if __name__ == "__main__":
    main()