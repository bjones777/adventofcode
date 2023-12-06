class Map:
    def __init__(self):
        self.map = []
        self.is_sorted = False
        
    def add(self, source: int, destination: int, length: int):
        self.map.append((source, destination, length))
        self.is_sorted = False

    def lookup(self, source_lookup: int) -> int:
        if not self.is_sorted:
            self.map.sort(key=lambda x: x[0])
            self.is_sorted = True
        for source, destination, length in self.map:
            if source_lookup >= source and source_lookup < source + length:
                return destination + source_lookup - source
            if source_lookup < source:
                break
        return source_lookup
    
    def get_mapped_ranges(self, start: int, len: int) -> [(int,int)]:        
        if not self.is_sorted:
            self.map.sort(key=lambda x: x[0])
            self.is_sorted = True
        mapped_ranges = []
        for source, destination, length in self.map:
            if len == 0:
                break
            if start >= source and start < source + length:
                end = min(start + len, source + length)
                l = end - start
                mapped_ranges.append((destination + start - source, l))
                start += l
                len -= l
            if start < source and start + len > source:
                l = source - start
                mapped_ranges.append((start, l))
                start = start + l
                len -= l
        if len > 0:
            mapped_ranges.append((start, len))
        return mapped_ranges
    
def read_seeds(line: str) -> ([int],int):
    colon_index = line.index(":")
    starting_point = colon_index + 1

    seed_strs = line[starting_point:]
    return [int(seed.strip()) for seed in seed_strs.split(" ") if seed.strip() != ""]

def part_a(lines : [str]) -> int:
    line_index = 0
    seeds = read_seeds(lines[0])
    line_index += 1
    maps = []
    for i in range(0,7):
        map = Map()
        line_index += 1
        #print(f"Reading map {i}: {lines[line_index]}")
        line_index += 1
        while line_index < len(lines) and lines[line_index] != "":
            destination, source, length = [int(num.strip()) for num in lines[line_index].split(" ") if num.strip() != ""]
            map.add(source, destination, length)
            line_index += 1
        #print(f"{len(map.map)} entries")
        maps.append(map)

    min_val = None
    for seed in seeds:
        
        for i in range(0,7):
            seed = maps[i].lookup(seed)
        if min_val is None or seed < min_val:
            min_val = seed
    return min_val


def eval_range(start: int, len: int, maps: [Map]) -> int:
    mapped_ranges = [(start, len)]
    for map in maps:
        new_mapped_ranges = []
        for mapped_range in mapped_ranges:
            new_mapped_ranges.extend(map.get_mapped_ranges(mapped_range[0], mapped_range[1]))
        mapped_ranges = new_mapped_ranges
    min_val = None
    for mapped_range in mapped_ranges:
        if min_val is None or mapped_range[0] < min_val:
            min_val = mapped_range[0]
    return min_val


def part_b(lines : [str]) -> int:
    line_index = 0
    seeds = read_seeds(lines[0])
    line_index += 1
    maps = []
    for i in range(0,7):
        map = Map()
        line_index += 1
        #print(f"Reading map {i}: {lines[line_index]}")
        line_index += 1
        while line_index < len(lines) and lines[line_index] != "":
            destination, source, length = [int(num.strip()) for num in lines[line_index].split(" ") if num.strip() != ""]
            map.add(source, destination, length)
            line_index += 1
        #print(f"{len(map.map)} entries")
        maps.append(map)

    min_val = None
    for i in range(0,len(seeds), 2):
        local_min = eval_range(seeds[i], seeds[i+1], maps)        
        if min_val is None or local_min < min_val:
            min_val = local_min
    return min_val

def main():
    lines = []
    with open("day05.txt") as file:
        for line in file:
            line = line.strip()
            lines.append(line)
    print(f"Part A: {part_a(lines)}")    
    print(f"Part B: {part_b(lines)}")    
    
if __name__ == "__main__":
    main()