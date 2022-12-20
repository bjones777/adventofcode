use aoc_2022::*;

fn mix(encrypted : &mut Vec<(i32,usize)>, i : usize) {
    let mut src_index = 0;
    loop {
        if encrypted[src_index].1 == i {
            break;
        }
        src_index += 1;
    }
    let n = encrypted.len();
    let mut num_moves = encrypted[src_index].0;
    if num_moves >= 0 {
        num_moves %= (n-1) as i32;
    }
    else {
        num_moves = -num_moves;
        num_moves %= (n-1) as i32;
        num_moves = -num_moves;
    }
    while num_moves != 0 {
        if num_moves < 0 {
            if src_index == 0 {
                let v = encrypted[src_index];
                for i in 0..n-1 {
                    encrypted[i] = encrypted[i+1];
                }
                encrypted[n-1] = v;
                encrypted.swap(n-1,n-2);
                src_index = n-2;
            }
            else {
                encrypted.swap(src_index,src_index-1);
                src_index -= 1;
            }
            num_moves += 1;
        }
        else {
            if src_index == n-1 {
                encrypted.swap(0,n-1);
                src_index = 0;
            }
            else {
                encrypted.swap(src_index,src_index+1);
                src_index += 1;
            }
            num_moves -= 1;
        }
    }   
}

fn part_a(lines: &Vec<i32>) -> i32 {
    let mut encrypted : Vec<(i32,usize)> = Vec::new();
    for t in lines.iter().enumerate() {
        encrypted.push((*t.1,t.0));
    }
    for i in 0..lines.len() {
        mix(&mut encrypted,i);
    }
    let mut zero_index = 0;
    loop {
        if encrypted[zero_index].0 == 0 {
            break;
        }
        zero_index += 1;
    }
    let n = encrypted.len();
    encrypted[(zero_index+1000) % n].0 + encrypted[(zero_index+2000) % n].0 + encrypted[(zero_index+3000) % n].0
}

fn mix2(encrypted : &mut Vec<(i64,usize)>, i : usize) {
    let mut src_index = 0;
    loop {
        if encrypted[src_index].1 == i {
            break;
        }
        src_index += 1;
    }
    let n = encrypted.len();
    let mut num_moves = encrypted[src_index].0;
    if num_moves >= 0 {
        num_moves %= (n-1) as i64;
    }
    else {
        num_moves = -num_moves;
        num_moves %= (n-1) as i64;
        num_moves = -num_moves;
    }
    while num_moves != 0 {
        if num_moves < 0 {
            if src_index == 0 {
                let v = encrypted[src_index];
                for i in 0..n-1 {
                    encrypted[i] = encrypted[i+1];
                }
                encrypted[n-1] = v;
                encrypted.swap(n-1,n-2);
                src_index = n-2;
            }
            else {
                encrypted.swap(src_index,src_index-1);
                src_index -= 1;
            }
            num_moves += 1;
        }
        else {
            if src_index == n-1 {
                encrypted.swap(0,n-1);
                src_index = 0;
            }
            else {
                encrypted.swap(src_index,src_index+1);
                src_index += 1;
            }
            num_moves -= 1;
        }
    }   
}

fn part_b(lines: &Vec<i32>) -> i64 {
    let mut encrypted : Vec<(i64,usize)> = Vec::new();
    for t in lines.iter().enumerate() {
        encrypted.push(((*t.1 as i64)*811589153i64,t.0));
    }
    for _r in 0..10 {
        for i in 0..lines.len() {
            mix2(&mut encrypted,i);
        }
    }
    let mut zero_index = 0;
    loop {
        if encrypted[zero_index].0 == 0 {
            break;
        }
        zero_index += 1;
    }
    let n = encrypted.len();
    encrypted[(zero_index+1000) % n].0 + encrypted[(zero_index+2000) % n].0 + encrypted[(zero_index+3000) % n].0
}

fn main() {
    let lines: Vec<i32> = read_lines("./day20.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            s.as_str().parse::<i32>().unwrap()
        })
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
