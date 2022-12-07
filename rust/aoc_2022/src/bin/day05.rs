use aoc_2022::*;
use regex::Regex;

fn part_a(mut stacks: Vec<Vec<char>>, lines: &Vec<(i32, i32, i32)>) -> String {
    for x in lines {
        let a = x.0;
        let f = (x.1 - 1) as usize;
        let t = (x.2 - 1) as usize;
        for _ in 0..a {
            let v = stacks[f].pop().unwrap();
            stacks[t].push(v);
        }
    }
    let mut ret_val = String::new();
    for s in &stacks {
        if !s.is_empty() {
            ret_val.push(*s.last().unwrap());
        }
    }
    ret_val
}

fn part_b(mut stacks: Vec<Vec<char>>, lines: &Vec<(i32, i32, i32)>) -> String {
    for x in lines {
        let a = x.0;
        let f = (x.1 - 1) as usize;
        let t = (x.2 - 1) as usize;
        let mut ns = Vec::new();
        for _ in 0..a {
            let v = stacks[f].pop().unwrap();
            ns.push(v);
        }
        for x in ns.iter().rev() {
            stacks[t].push(*x);
        }
    }
    let mut ret_val = String::new();
    for s in &stacks {
        if !s.is_empty() {
            ret_val.push(*s.last().unwrap());
        }
    }
    ret_val
}

fn main() {
    /* this is taken from my input
            [H]         [S]         [D]
        [S] [C]         [C]     [Q] [L]
        [C] [R] [Z]     [R]     [H] [Z]
        [G] [N] [H] [S] [B]     [R] [F]
    [D] [T] [Q] [F] [Q] [Z]     [Z] [N]
    [Z] [W] [F] [N] [F] [W] [J] [V] [G]
    [T] [R] [B] [C] [L] [P] [F] [L] [H]
    [H] [Q] [P] [L] [G] [V] [Z] [D] [B]
     1   2   3   4   5   6   7   8   9

         */
    let stacks: Vec<Vec<char>> = vec![
        vec!['H', 'T', 'Z', 'D'],
        vec!['Q', 'R', 'W', 'T', 'G', 'C', 'S'],
        vec!['P', 'B', 'F', 'Q', 'N', 'R', 'C', 'H'],
        vec!['L', 'C', 'N', 'F', 'H', 'Z'],
        vec!['G', 'L', 'F', 'Q', 'S'],
        vec!['V', 'P', 'W', 'Z', 'B', 'R', 'C', 'S'],
        vec!['Z', 'F', 'J'],
        vec!['D', 'L', 'V', 'Z', 'R', 'H', 'Q'],
        vec!['B', 'H', 'G', 'N', 'F', 'Z', 'L', 'D'],
    ];

    let re = Regex::new(r"move (\d+) from (\d+) to (\d+)").unwrap();
    let lines: Vec<(i32, i32, i32)> = read_lines("./day05.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            for caps in re.captures_iter(s.as_str()) {
                return (
                    caps.get(1).unwrap().as_str().parse::<i32>().unwrap(),
                    caps.get(2).unwrap().as_str().parse::<i32>().unwrap(),
                    caps.get(3).unwrap().as_str().parse::<i32>().unwrap(),
                );
            }
            panic!("Pattern didn't match!");
        })
        .collect();
    println!("Part A: {}", part_a(stacks.clone(), &lines));
    println!("Part B: {}", part_b(stacks.clone(), &lines));
}
