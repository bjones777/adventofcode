use aoc_2022::*;

use std::collections::HashSet;

fn part_a(lines: &Vec<(i32,i32,i32)>) -> i32 {
    let mut hash_set : HashSet<(i32,i32,i32)> = HashSet::new();
    for t in lines {
        hash_set.insert(*t);
    }
    let mut total_sa = 0;
    for t in lines {
        let mut sa = 0;
        if !hash_set.contains(&(t.0-1,t.1,t.2)) {
            sa += 1;
        }
        if !hash_set.contains(&(t.0+1,t.1,t.2)) {
            sa += 1;
        }
        if !hash_set.contains(&(t.0,t.1-1,t.2)) {
            sa += 1;
        }
        if !hash_set.contains(&(t.0,t.1+1,t.2)) {
            sa += 1;
        }
        if !hash_set.contains(&(t.0,t.1,t.2-1)) {
            sa += 1;
        }
        if !hash_set.contains(&(t.0,t.1,t.2+1)) {
            sa += 1;
        }
        total_sa += sa;
    }

    total_sa
}

fn inbounds(p: &(i32,i32,i32),min: &(i32,i32,i32),max: &(i32,i32,i32)) -> bool {
    if !(p.0 >= min.0 && p.0 <= max.0) {
        return false;
    }
    if !(p.1 >= min.1 && p.1 <= max.1) {
        return false;
    }
    if !(p.2 >= min.2 && p.2 <= max.2) {
        return false;
    }
    true
}

fn part_b(lines: &Vec<(i32,i32,i32)>) -> i32 {
    let mut min_coords = *lines.first().unwrap();
    let mut max_coords = *lines.first().unwrap();
    for t in lines {
        min_coords.0 = i32::min(min_coords.0,t.0);
        min_coords.1 = i32::min(min_coords.1,t.1);
        min_coords.2 = i32::min(min_coords.2,t.2);

        max_coords.0 = i32::max(max_coords.0,t.0);
        max_coords.1 = i32::max(max_coords.1,t.1);
        max_coords.2 = i32::max(max_coords.2,t.2);
    }
    min_coords.0 -= 1;
    min_coords.1 -= 1;
    min_coords.2 -= 1;

    max_coords.0 += 1;
    max_coords.1 += 1;
    max_coords.2 += 1;

    let mut rock_set : HashSet<(i32,i32,i32)> = HashSet::new();
    for t in lines {
        rock_set.insert(*t);
    }

    let mut water_set : HashSet<(i32,i32,i32)> = HashSet::new();
    for x in min_coords.0..=max_coords.0 {
        for y in min_coords.1..=max_coords.1 {
            water_set.insert((x,y,min_coords.2));
            water_set.insert((x,y,max_coords.2));
        }
    }

    for x in min_coords.0..=max_coords.0 {
        for z in min_coords.2..=max_coords.2 {
            water_set.insert((x,min_coords.1,z));
            water_set.insert((x,max_coords.1,z));
        }
    }

    for y in min_coords.1..=max_coords.1 {
        for z in min_coords.2..=max_coords.2 {
            water_set.insert((min_coords.0,y,z));
            water_set.insert((max_coords.0,y,z));
        }
    }

    loop {
        let mut water_to_add : Vec<(i32,i32,i32)> = Vec::new();
        for t in water_set.iter() {
            if !rock_set.contains(&(t.0-1,t.1,t.2)) && !water_set.contains(&(t.0-1,t.1,t.2)) && inbounds(&(t.0-1,t.1,t.2),&min_coords,&max_coords) {
                water_to_add.push((t.0-1,t.1,t.2));
            }
            if !rock_set.contains(&(t.0+1,t.1,t.2)) && !water_set.contains(&(t.0+1,t.1,t.2)) && inbounds(&(t.0+1,t.1,t.2),&min_coords,&max_coords){
                water_to_add.push((t.0+1,t.1,t.2));
            }
            if !rock_set.contains(&(t.0,t.1-1,t.2)) && !water_set.contains(&(t.0,t.1-1,t.2)) && inbounds(&(t.0,t.1-1,t.2),&min_coords,&max_coords){
                water_to_add.push((t.0,t.1-1,t.2));
            }
            if !rock_set.contains(&(t.0,t.1+1,t.2)) && !water_set.contains(&(t.0,t.1+1,t.2)) && inbounds(&(t.0,t.1+1,t.2),&min_coords,&max_coords){
                water_to_add.push((t.0,t.1+1,t.2));
            }
            if !rock_set.contains(&(t.0,t.1,t.2-1)) && !water_set.contains(&(t.0,t.1,t.2-1)) && inbounds(&(t.0,t.1,t.2-1),&min_coords,&max_coords){
                water_to_add.push((t.0,t.1,t.2-1));
            }
            if !rock_set.contains(&(t.0,t.1,t.2+1)) && !water_set.contains(&(t.0,t.1,t.2+1)) && inbounds(&(t.0,t.1,t.2+1),&min_coords,&max_coords){
                water_to_add.push((t.0,t.1,t.2+1));
            }
        }
        if water_to_add.is_empty() {
            break;
        }
        for t in water_to_add {
            water_set.insert(t);
        }
    }

    let mut total_sa = 0;
    for t in lines {
        let mut sa = 0;
        if water_set.contains(&(t.0-1,t.1,t.2)) {
            sa += 1;
        }
        if water_set.contains(&(t.0+1,t.1,t.2)) {
            sa += 1;
        }
        if water_set.contains(&(t.0,t.1-1,t.2)) {
            sa += 1;
        }
        if water_set.contains(&(t.0,t.1+1,t.2)) {
            sa += 1;
        }
        if water_set.contains(&(t.0,t.1,t.2-1)) {
            sa += 1;
        }
        if water_set.contains(&(t.0,t.1,t.2+1)) {
            sa += 1;
        }
        total_sa += sa;
    }
    total_sa
}

fn main() {
    let lines: Vec<(i32,i32,i32)> = read_lines("./day18.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            let x : Vec<&str> = s.as_str().split(",").collect();
            (x[0].parse::<i32>().unwrap(), x[1].parse::<i32>().unwrap(), x[2].parse::<i32>().unwrap())
        })
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
