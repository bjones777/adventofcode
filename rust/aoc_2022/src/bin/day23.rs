use aoc_2022::*;

use std::collections::HashMap;
use std::collections::HashSet;

fn has_neighbor(elf_set: &HashSet<(i32, i32)>, elf: &(i32, i32)) -> bool {
    for dr in -1..=1 {
        for dc in -1..=1 {
            if dr == 0 && dc == 0 {
                continue;
            }
            if elf_set.contains(&(elf.0 + dr, elf.1 + dc)) {
                return true;
            }
        }
    }
    false
}

fn should_move_north(elf_set: &HashSet<(i32, i32)>, elf: &(i32, i32)) -> bool {
    let dr = -1;
    for dc in -1..=1 {
        if elf_set.contains(&(elf.0 + dr, elf.1 + dc)) {
            return false;
        }
    }
    true
}

fn should_move_south(elf_set: &HashSet<(i32, i32)>, elf: &(i32, i32)) -> bool {
    let dr = 1;
    for dc in -1..=1 {
        if elf_set.contains(&(elf.0 + dr, elf.1 + dc)) {
            return false;
        }
    }
    true
}

fn should_move_east(elf_set: &HashSet<(i32, i32)>, elf: &(i32, i32)) -> bool {
    let dc = 1;
    for dr in -1..=1 {
        if elf_set.contains(&(elf.0 + dr, elf.1 + dc)) {
            return false;
        }
    }
    true
}

fn should_move_west(elf_set: &HashSet<(i32, i32)>, elf: &(i32, i32)) -> bool {
    let dc = -1;
    for dr in -1..=1 {
        if elf_set.contains(&(elf.0 + dr, elf.1 + dc)) {
            return false;
        }
    }
    true
}

#[allow(dead_code)]
fn print(elf_set: &HashSet<(i32, i32)>) {
    let mut mins = (i32::MAX, i32::MAX);
    let mut maxs = (i32::MIN, i32::MIN);
    for elf in elf_set.iter() {
        mins = (i32::min(mins.0, elf.0), i32::min(mins.1, elf.1));
        maxs = (i32::max(maxs.0, elf.0), i32::max(maxs.1, elf.1));
    }
    for r in mins.0..=maxs.0 {
        let mut s = String::new();
        for c in mins.1..=maxs.1 {
            if !elf_set.contains(&(r, c)) {
                s.push('.');
            } else {
                s.push('#');
            }
        }
        println!("{}", s);
    }
    println!("");
}

fn do_round(elf_set: &HashSet<(i32, i32)>, round: usize) -> HashSet<(i32, i32)> {
    let mut new_elf_set: HashSet<(i32, i32)> = HashSet::new();

    let mut proposed_moves: HashMap<(i32, i32), (i32, i32)> = HashMap::new();
    let mut new_loc_counts: HashMap<(i32, i32), usize> = HashMap::new();

    for elf in elf_set.iter() {
        if !has_neighbor(elf_set, elf) {
            continue;
        }
        let valid_moves: [bool; 4] = [
            should_move_north(elf_set, elf),
            should_move_south(elf_set, elf),
            should_move_west(elf_set, elf),
            should_move_east(elf_set, elf),
        ];
        let start_index = round % 4;
        let mut first_true: Option<usize> = None;
        for i in 0..4 {
            let index = (start_index + i) % 4;
            if valid_moves[index] {
                first_true = Some(index);
                break;
            }
        }
        if first_true.is_none() {
            continue;
        }
        let dir = first_true.unwrap();
        let new_loc = if dir == 0 {
            (elf.0 - 1, elf.1)
        } else if dir == 1 {
            (elf.0 + 1, elf.1)
        } else if dir == 2 {
            (elf.0, elf.1 - 1)
        } else {
            assert!(dir == 3);
            (elf.0, elf.1 + 1)
        };
        if new_loc == *elf {
            continue;
        }
        proposed_moves.insert(*elf, new_loc);
        *new_loc_counts.entry(new_loc).or_insert(0) += 1;
    }

    for elf in elf_set.iter() {
        match proposed_moves.get(elf) {
            Some(new_loc) => {
                let count = *new_loc_counts.get(new_loc).unwrap();
                if count == 1 {
                    new_elf_set.insert(*new_loc);
                } else {
                    new_elf_set.insert(*elf);
                }
            }
            None => {
                new_elf_set.insert(*elf);
            }
        }
    }
    new_elf_set
}

fn part_a(lines: &Vec<String>) -> i32 {
    let mut elf_set: HashSet<(i32, i32)> = HashSet::new();
    for (r, line) in lines.iter().enumerate() {
        for (c, ch) in line.chars().enumerate() {
            if ch == '#' {
                elf_set.insert((r as i32, c as i32));
            }
        }
    }
    for i in 0..10 {
        elf_set = do_round(&elf_set, i);
    }

    let mut mins = (i32::MAX, i32::MAX);
    let mut maxs = (i32::MIN, i32::MIN);
    for elf in elf_set.iter() {
        mins = (i32::min(mins.0, elf.0), i32::min(mins.1, elf.1));
        maxs = (i32::max(maxs.0, elf.0), i32::max(maxs.1, elf.1));
    }
    let mut count = 0;
    for r in mins.0..=maxs.0 {
        for c in mins.1..=maxs.1 {
            if !elf_set.contains(&(r, c)) {
                count += 1;
            }
        }
    }

    count
}

fn part_b(lines: &Vec<String>) -> usize {
    let mut elf_set: HashSet<(i32, i32)> = HashSet::new();
    for (r, line) in lines.iter().enumerate() {
        for (c, ch) in line.chars().enumerate() {
            if ch == '#' {
                elf_set.insert((r as i32, c as i32));
            }
        }
    }
    let mut round = 0;
    loop {
        let new_elf_set = do_round(&elf_set, round);
        round += 1;
        if new_elf_set == elf_set {
            return round;
        }
        elf_set = new_elf_set;
    }
}

fn main() {
    let lines: Vec<String> = read_lines("./day23.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
