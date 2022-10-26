use aoc_2021::*;

fn part_a(commands: &Vec<(String, i32)>) -> i32 {
    struct State {
        depth: i32,
        horizontal: i32,
    }
    let mut state = State {
        depth: 0,
        horizontal: 0,
    };
    for cmd in commands {
        match cmd.0.as_str() {
            "forward" => state.horizontal += cmd.1,
            "up" => state.depth -= cmd.1,
            "down" => state.depth += cmd.1,
            _ => panic!("unknown cmd {}", &cmd.0),
        }
    }
    state.depth * state.horizontal
}

fn part_b(commands: &Vec<(String, i32)>) -> i64 {
    struct State {
        depth: i64,
        horizontal: i64,
        aim: i64,
    }
    let mut state = State {
        depth: 0,
        horizontal: 0,
        aim: 0,
    };
    for cmd in commands {
        let delta = cmd.1 as i64;
        match cmd.0.as_str() {
            "forward" => {
                state.horizontal += delta;
                state.depth += state.aim * delta;
            }
            "up" => state.aim -= delta,
            "down" => state.aim += delta,
            _ => panic!("unknown cmd {}", &cmd.0),
        }
    }
    state.depth * state.horizontal
}

fn main() {
    let commands: Vec<(String, i32)> = read_lines("./day02.txt")
        .unwrap()
        .map(|l| l.unwrap())
        .map(|l| {
            let parts: Vec<&str> = l.split_whitespace().collect();
            assert!(parts.len() == 2);
            (String::from(parts[0]), parts[1].parse::<i32>().unwrap())
        })
        .collect();
    println!("Part A: {}", part_a(&commands));
    println!("Part B: {}", part_b(&commands));
}
