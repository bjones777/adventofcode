use aoc_2022::*;

use std::collections::HashSet;

fn part_a(lines: &Vec<Vec<i32>>) -> usize {
    let m = lines.len();
    let n = lines.first().unwrap().len();
    let mut visible_interior: HashSet<(usize, usize)> = HashSet::new();

    // left
    for r in 1..(m - 1) {
        let mut max_height = lines[r][0];
        for c in 1..(n - 1) {
            let height = lines[r][c];
            if height > max_height {
                max_height = height;
                visible_interior.insert((r, c));
            }
        }
    }

    // right
    for r in 1..(m - 1) {
        let mut max_height = lines[r][n - 1];
        for c in (1..(n - 1)).rev() {
            let height = lines[r][c];
            if height > max_height {
                max_height = height;
                visible_interior.insert((r, c));
            }
        }
    }

    // top
    for c in 1..(n - 1) {
        let mut max_height = lines[0][c];
        for r in 1..(m - 1) {
            let height = lines[r][c];
            if height > max_height {
                max_height = height;
                visible_interior.insert((r, c));
            }
        }
    }

    // bottom
    for c in 1..(n - 1) {
        let mut max_height = lines[m - 1][c];
        for r in (1..(m - 1)).rev() {
            let height = lines[r][c];
            if height > max_height {
                max_height = height;
                visible_interior.insert((r, c));
            }
        }
    }

    (m * 2) + (n - 2) * 2 + visible_interior.len()
}

fn calc_score_dir(lines: &Vec<Vec<i32>>, r: usize, c: usize, dr: i64, dc: i64) -> i64 {
    let m = lines.len() as i64;
    let n = lines.first().unwrap().len() as i64;

    let mut dist = 0i64;
    let height = lines[r][c];
    let mut nr = r as i64 + dr;
    let mut nc = c as i64 + dc;
    while nc >= 0 && nc < n && nr >= 0 && nr < m {
        dist += 1;
        let new_height = lines[nr as usize][nc as usize];
        if new_height >= height {
            break;
        }
        nr += dr;
        nc += dc;
    }
    dist
}

fn calc_score(lines: &Vec<Vec<i32>>, r: usize, c: usize) -> i64 {
    calc_score_dir(&lines, r, c, 0, 1)
        * calc_score_dir(&lines, r, c, 0, -1)
        * calc_score_dir(&lines, r, c, 1, 0)
        * calc_score_dir(&lines, r, c, -1, 0)
}

fn part_b(lines: &Vec<Vec<i32>>) -> i64 {
    let m = lines.len();
    let n = lines.first().unwrap().len();
    let mut max_score = i64::MIN;
    for r in 0..m {
        for c in 0..n {
            let score = calc_score(&lines, r, c);
            if score > max_score {
                max_score = score;
            }
        }
    }
    max_score
}

fn main() {
    let lines: Vec<Vec<i32>> = read_lines("./day08.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            let mut ret_val: Vec<i32> = Vec::new();
            for ch in s.chars() {
                let mut s = String::new();
                s.push(ch);
                ret_val.push(s.parse::<i32>().unwrap());
            }
            ret_val
        })
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
