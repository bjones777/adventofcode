use aoc_2022::*;

const MARKER: i32 = -1;

fn part_a(lines: &Vec<i32>) -> i32 {
    let mut max_sum = -1;
    let mut current_sum = 0;
    for v in lines {
        if *v == MARKER {
            max_sum = i32::max(max_sum, current_sum);
            current_sum = 0;
        } else {
            current_sum += *v;
        }
    }
    max_sum = i32::max(max_sum, current_sum);
    max_sum
}

fn part_b(lines: &Vec<i32>) -> i32 {
    let mut quant: Vec<i32> = Vec::new();
    let mut current_sum = 0;
    for v in lines {
        if *v == MARKER {
            quant.push(current_sum);
            current_sum = 0;
        } else {
            current_sum += *v;
        }
    }
    quant.push(current_sum);
    quant.sort();
    let n = quant.len();
    quant[n - 1] + quant[n - 2] + quant[n - 3]
}

fn main() {
    let lines: Vec<i32> = read_lines("./day01.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            if s.is_empty() {
                MARKER
            } else {
                s.parse::<i32>().unwrap()
            }
        })
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
