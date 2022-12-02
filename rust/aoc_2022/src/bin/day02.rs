use aoc_2022::*;

fn part_a(lines: &Vec<(String, String)>) -> i32 {
    lines
        .iter()
        .map(|t| match t.0.as_str() {
            "A" => match t.1.as_str() {
                "X" => 3 + 1,
                "Y" => 6 + 2,
                "Z" => 0 + 3,
                _ => panic!("Unknown!"),
            },
            "B" => match t.1.as_str() {
                "X" => 0 + 1,
                "Y" => 3 + 2,
                "Z" => 6 + 3,
                _ => panic!("Unknown!"),
            },
            "C" => match t.1.as_str() {
                "X" => 6 + 1,
                "Y" => 0 + 2,
                "Z" => 3 + 3,
                _ => panic!("Unknown!"),
            },
            _ => panic!("Unknown"),
        })
        .sum()
}

fn part_b(lines: &Vec<(String, String)>) -> i32 {
    lines
        .iter()
        .map(|t| match t.0.as_str() {
            "A" => match t.1.as_str() {
                "X" => 0 + 3,
                "Y" => 3 + 1,
                "Z" => 6 + 2,
                _ => panic!("Unknown!"),
            },
            "B" => match t.1.as_str() {
                "X" => 0 + 1,
                "Y" => 3 + 2,
                "Z" => 6 + 3,
                _ => panic!("Unknown!"),
            },
            "C" => match t.1.as_str() {
                "X" => 0 + 2,
                "Y" => 3 + 3,
                "Z" => 6 + 1,
                _ => panic!("Unknown!"),
            },
            _ => panic!("Unknown"),
        })
        .sum()
}

fn main() {
    let lines: Vec<(String, String)> = read_lines("./day02.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            let v: Vec<String> = s
                .split_ascii_whitespace()
                .map(|s2| String::from(s2))
                .take(2)
                .collect();
            (v[0].clone(), v[1].clone())
        })
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
