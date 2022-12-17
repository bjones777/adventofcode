use aoc_2022::*;

use std::collections::HashSet;

fn build_shapes() -> Vec<HashSet<(i32,i32)>> {
    let mut ret_val :Vec<HashSet<(i32,i32)>> = Vec::new();
    {
        let mut shape : HashSet<(i32,i32)> = HashSet::new();
        shape.insert((0,0));
        shape.insert((1,0));
        shape.insert((2,0));
        shape.insert((3,0));
        ret_val.push(shape);
    }

    {
        let mut shape : HashSet<(i32,i32)> = HashSet::new();
        shape.insert((1,0));
        shape.insert((0,-1));
        shape.insert((1,-1));
        shape.insert((2,-1));
        shape.insert((1,-2));
        ret_val.push(shape);
    }

    {
        let mut shape : HashSet<(i32,i32)> = HashSet::new();
        shape.insert((2,0));
        shape.insert((2,-1));
        shape.insert((0,-2));
        shape.insert((1,-2));
        shape.insert((2,-2));
        ret_val.push(shape);
    }

    {
        let mut shape : HashSet<(i32,i32)> = HashSet::new();
        shape.insert((0,0));
        shape.insert((0,-1));
        shape.insert((0,-2));
        shape.insert((0,-3));
        ret_val.push(shape);
    }

    {
        let mut shape : HashSet<(i32,i32)> = HashSet::new();
        shape.insert((0,0));
        shape.insert((1,0));
        shape.insert((0,-1));
        shape.insert((1,-1));
        ret_val.push(shape);
    }
    ret_val
}

fn check_place(board: &HashSet<(i32,i32)>, shape: &HashSet<(i32,i32)>, coord: (i32,i32)) -> bool
{
    for c in shape.iter() {
        let pos = (c.0 + coord.0, c.1+coord.1);
        if pos.0 < 0 || pos.0 >= 7 {
            return false;
        }
        if pos.1 < 0 {
            return false;
        }
        if board.contains(&pos) {
            return false;
        }
    }
    true
}

fn place(board: &mut HashSet<(i32,i32)>, shape: &HashSet<(i32,i32)>, coord: (i32,i32)) -> bool
{
    for c in shape.iter() {
        let pos = (c.0 + coord.0, c.1+coord.1);
        let res = board.insert(pos);
        assert!(res);
    }
    true
}

fn get_bottom(shape: &HashSet<(i32,i32)>) -> i32 {
    let mut min_bottom = 0;
    for c in shape.iter() {
        min_bottom = i32::min(min_bottom, c.1);
    }
    min_bottom
}

fn part_a(jet_str: &String) -> i32 {
    let jet : Vec<char> = jet_str.chars().collect();
    let mut jet_index = 0;
    let mut top_line = 0;
    let mut shape_index = 0;
    let shapes = build_shapes();
    let mut board : HashSet<(i32,i32)> = HashSet::new();
    for _  in 0..2022 {
        let shape = &shapes[shape_index];
        shape_index += 1;
        if shape_index >= shapes.len() {
            shape_index = 0;
        }
        let mut cur_coord = (2,top_line-get_bottom(shape)+3);
        loop {
            let jet_dir = jet[jet_index];
            jet_index += 1;
            if jet_index >= jet.len() {
                jet_index = 0;
            }
            if jet_dir == '<' {
                if check_place(&mut board, shape, (cur_coord.0-1, cur_coord.1)) {
                    cur_coord = (cur_coord.0-1, cur_coord.1);
                }
            }
            else if jet_dir == '>' {
                if check_place(&mut board, shape, (cur_coord.0+1, cur_coord.1)) {
                    cur_coord = (cur_coord.0+1, cur_coord.1);
                }
            }
            if !check_place(&mut board, shape, (cur_coord.0, cur_coord.1-1)) {
                place(&mut board,shape,cur_coord);
                break;
            }
            cur_coord = (cur_coord.0, cur_coord.1-1);
        }
        top_line = i32::max(top_line, cur_coord.1+1);
    }
    top_line

}

fn sub(a: (i32,i32),b : (i32,i32)) -> (i32,i32) {
    (a.0 - b.0,a.1-b.1)
}

fn check_for_cycle(placements: &Vec<(i32,i32)>) -> Option<(i32,i32)> {
    let n = placements.len();
    let mut cycle_diff = 0;
    loop {
        cycle_diff += 1;
        if cycle_diff * 6 > n {
            return None;
        }
        let diff = sub(placements[n-1],placements[n-1-cycle_diff]);
        let diff1 = sub(placements[n-1-cycle_diff],placements[n-1-cycle_diff*2]); 
        if diff != diff1 {
            continue;
        }
        let diff2 = sub(placements[n-1-cycle_diff*2],placements[n-1-cycle_diff*3]);
        if diff == diff2 {
            return Some(diff);
        }
        let diff3 = sub(placements[n-1-cycle_diff*3],placements[n-1-cycle_diff*4]);
        if diff == diff3 {
            return Some(diff);
        }
        let diff4 = sub(placements[n-1-cycle_diff*3],placements[n-1-cycle_diff*4]);
        if diff == diff4 {
            return Some(diff);
        }
    }
}


fn part_b(jet_str: &String) -> i64 {
    let jet : Vec<char> = jet_str.chars().collect();
    let mut jet_index = 0;
    let mut top_line = 0;
    let mut shape_index = 0;
    let shapes = build_shapes();
    let mut jet_starts: Vec<Vec<Vec<(i32,i32)>>> = vec![vec![vec![];shapes.len()]; jet.len()];
    let mut board : HashSet<(i32,i32)> = HashSet::new();
    let mut shape_count = 0i64;
    loop {
        let shape = &shapes[shape_index];
        jet_starts[jet_index][shape_index].push((top_line, shape_count as i32));
        if let Some(cycle) = check_for_cycle(&jet_starts[jet_index][shape_index]) {
            let target = 1000000000000i64;
            let left = target - shape_count;
            if left % (cycle.1 as i64) == 0 {
                return (top_line as i64) + (cycle.0 as i64) * left/(cycle.1 as i64);
            }
        }
        shape_count += 1;
        shape_index += 1;
        if shape_index >= shapes.len() {
            shape_index = 0;
        }
        let mut cur_coord = (2,top_line-get_bottom(shape)+3);
        loop {
            let jet_dir = jet[jet_index];
            jet_index += 1;
            if jet_index >= jet.len() {
                jet_index = 0;
            }
            if jet_dir == '<' {
                if check_place(&mut board, shape, (cur_coord.0-1, cur_coord.1)) {
                    cur_coord = (cur_coord.0-1, cur_coord.1);
                }
            }
            else if jet_dir == '>' {
                if check_place(&mut board, shape, (cur_coord.0+1, cur_coord.1)) {
                    cur_coord = (cur_coord.0+1, cur_coord.1);
                }
            }
            if !check_place(&mut board, shape, (cur_coord.0, cur_coord.1-1)) {
                place(&mut board,shape,cur_coord);
                break;
            }
            cur_coord = (cur_coord.0, cur_coord.1-1);
        }
        top_line = i32::max(top_line, cur_coord.1+1);
    }
}

fn main() {
    let lines: Vec<String> = read_lines("./day17.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .collect();
    println!("Part A: {}", part_a(lines.first().unwrap()));
    // too low 1532163742756
    println!("Part B: {}", part_b(lines.first().unwrap()));
}
