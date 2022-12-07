use aoc_2022::*;

fn has_matching(chs: &[char]) -> bool {
    use std::collections::HashSet;
    let mut set: HashSet<char> = HashSet::new();
    for ch in chs {
        if !set.insert(*ch) {
            return true;
        }
    }
    false
}

fn part_a(line: &String) -> usize {
    let n = line.len();
    let char_vec: Vec<char> = line.chars().collect();
    for x in 0..(n - 4) {
        let chs = &char_vec[x..x + 4];
        if !has_matching(chs) {
            return x + 4;
        }
    }
    panic!("Not found")
}

fn part_b(line: &String) -> usize {
    let n = line.len();
    let char_vec: Vec<char> = line.chars().collect();
    for x in 0..(n - 14) {
        let chs = &char_vec[x..x + 14];
        if !has_matching(chs) {
            return x + 14;
        }
    }
    panic!("Not found")
}

fn main() {
    let lines: Vec<String> = read_lines("./day06.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .collect();
    let line = lines.first().unwrap();
    println!("Part A: {}", part_a(&line));
    println!("Part B: {}", part_b(&line));
}
