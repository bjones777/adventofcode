use aoc_2022::*;

fn part_a(lines: &Vec<((i32, i32), (i32, i32))>) -> i32 {
    lines
        .iter()
        .map(|t| {
            let a = t.0;
            let b = t.1;

            if (a.0 <= b.0 && a.1 >= b.1) || (b.0 <= a.0 && b.1 >= a.1) {
                1
            } else {
                0
            }
        })
        .sum()
}

fn part_b(lines: &Vec<((i32, i32), (i32, i32))>) -> i32 {
    lines
        .iter()
        .map(|t| {
            let a = t.0;
            let b = t.1;
            if (a.0 <= b.0 && a.1 >= b.0) || (b.0 <= a.0 && b.1 >= a.0) {
                1
            } else {
                0
            }
        })
        .sum()
}

fn main() {
    let lines: Vec<((i32, i32), (i32, i32))> = read_lines("./day04.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            if let [r1, r2] = &s.split(",").collect::<Vec<&str>>()[..] {
                if let [s1, e1] = &r1.split("-").collect::<Vec<&str>>()[..] {
                    if let [s2, e2] = r2.split("-").collect::<Vec<&str>>()[..] {
                        return (
                            (s1.parse::<i32>().unwrap(), e1.parse::<i32>().unwrap()),
                            (s2.parse::<i32>().unwrap(), e2.parse::<i32>().unwrap()),
                        );
                    } else {
                        panic!("couldn't split at 2 -");
                    }
                } else {
                    panic!("couldn't split at 1 -");
                }
            } else {
                panic!("couldn't split a ,");
            }
        })
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
