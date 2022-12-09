use aoc_2022::*;

use std::collections::HashSet;

fn do_move(head_pos: &mut (i32, i32), tail_pos: &mut (i32, i32), dir: char) {
    let mut new_head_pos = *head_pos;
    match dir {
        'U' => new_head_pos.1 += 1,
        'D' => new_head_pos.1 -= 1,
        'L' => new_head_pos.0 -= 1,
        'R' => new_head_pos.0 += 1,
        _ => panic!("Unknown dir"),
    }
    let x_diff = (new_head_pos.0 - tail_pos.0).abs();
    let y_diff = (new_head_pos.1 - tail_pos.1).abs();
    if !(x_diff <= 1 && y_diff <= 1) {
        *tail_pos = *head_pos;
    }
    *head_pos = new_head_pos;
}

fn do_move_b(pos: Vec<(i32, i32)>, dir: char) -> Vec<(i32, i32)> {
    let mut new_pos = pos.clone();
    match dir {
        'U' => new_pos.first_mut().unwrap().1 += 1,
        'D' => new_pos.first_mut().unwrap().1 -= 1,
        'L' => new_pos.first_mut().unwrap().0 -= 1,
        'R' => new_pos.first_mut().unwrap().0 += 1,
        _ => panic!("Unknown dir"),
    }
    for i in 1..pos.len() {
        let new_head_pos = new_pos[i - 1];
        let tail_pos = new_pos[i];
        let x_diff = (new_head_pos.0 - tail_pos.0).abs();
        let y_diff = (new_head_pos.1 - tail_pos.1).abs();
        if !(x_diff <= 1 && y_diff <= 1) {
            if x_diff == 0 {
                let dir = if new_head_pos.1 > tail_pos.1 { 1 } else { -1 };
                new_pos[i] = (tail_pos.0, tail_pos.1 + dir);
            } else if y_diff == 0 {
                let dir = if new_head_pos.0 > tail_pos.0 { 1 } else { -1 };
                new_pos[i] = (tail_pos.0 + dir, tail_pos.1);
            } else {
                if new_head_pos.0 > tail_pos.0 && new_head_pos.1 > tail_pos.1 {
                    new_pos[i] = (tail_pos.0 + 1, tail_pos.1 + 1);
                } else if new_head_pos.0 > tail_pos.0 && new_head_pos.1 < tail_pos.1 {
                    new_pos[i] = (tail_pos.0 + 1, tail_pos.1 - 1);
                } else if new_head_pos.0 < tail_pos.0 && new_head_pos.1 < tail_pos.1 {
                    new_pos[i] = (tail_pos.0 - 1, tail_pos.1 - 1);
                } else {
                    new_pos[i] = (tail_pos.0 - 1, tail_pos.1 + 1);
                }
            }
        } else {
            break;
        }
    }
    new_pos
}

fn part_a(lines: &Vec<(char, i32)>) -> usize {
    let mut head_pos = (0, 0);
    let mut tail_pos = (0, 0);
    let mut tail_locations: HashSet<(i32, i32)> = HashSet::new();
    tail_locations.insert(tail_pos);
    for t in lines.iter() {
        for _ in 0..t.1 {
            do_move(&mut head_pos, &mut tail_pos, t.0);
            tail_locations.insert(tail_pos);
        }
    }
    tail_locations.len()
}

fn part_b(lines: &Vec<(char, i32)>) -> usize {
    let mut knots = vec![(0, 0); 10];
    let mut tail_locations: HashSet<(i32, i32)> = HashSet::new();
    tail_locations.insert(*knots.last().unwrap());
    for t in lines.iter() {
        for _ in 0..t.1 {
            knots = do_move_b(knots, t.0);
            tail_locations.insert(*knots.last().unwrap());
        }
    }
    tail_locations.len()
}

fn main() {
    let lines: Vec<(char, i32)> = read_lines("./day09.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            let parts: Vec<&str> = s.split_ascii_whitespace().collect();
            (
                parts[0].chars().next().unwrap(),
                parts[1].parse::<i32>().unwrap(),
            )
        })
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
