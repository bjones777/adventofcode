use aoc_2022::*;
use std::collections::HashMap;

enum TileType {
    Rock,
    Sand,
}

fn draw_line_from_points(a: (i32,i32),b: (i32,i32), board : &mut HashMap<(i32,i32),TileType>) {
    if a.0 == b.0 {
        let dy = if b.1 > a.1 { 1 } else { -1 };
        let mut p = a;
        while p != b {
            board.insert(p, TileType::Rock);
            p.1 += dy;
        }
        board.insert(b, TileType::Rock);
    }
    else {
        assert!(a.1 == b.1);
        let dx = if b.0 > a.0 { 1 } else { -1 };
        let mut p = a;
        while p != b {
            board.insert(p, TileType::Rock);
            p.0 += dx;
        }
        board.insert(b, TileType::Rock);
    }
}

fn draw_line(line : &Vec<(i32,i32)>, board : &mut HashMap<(i32,i32),TileType>) {
    line.windows(2).for_each(|x| {
        draw_line_from_points(x[0], x[1], board);
    });
}

fn drop_sand(board : &mut HashMap<(i32,i32),TileType>,start_loc : (i32,i32), max_y : i32) -> bool {
    let mut p = start_loc;
    loop {
        let mut np = (p.0,p.1+1);
        if np.1 > max_y {
            return false;
        }
        if board.get(&np).is_none() {
            p = np;
            continue;
        }
        np.0 -= 1;
        if board.get(&np).is_none() {
            p = np;
            continue;
        }
        np.0 += 2;
        if board.get(&np).is_none() {
            p = np;
            continue;
        }
        board.insert(p,TileType::Sand);
        return true;
    }
}

fn part_a(lines: &Vec<Vec<(i32,i32)>>) -> i32 {
    let mut board : HashMap<(i32,i32),TileType> = HashMap::new();
    for line in lines {
        draw_line(line,&mut board);
    }
    let max_y = lines.iter().flatten().map(|x| x.1).max().unwrap();
    let mut sand_count = 0;
    while drop_sand(&mut board, (500,0), max_y) {
        sand_count += 1;
    }
    sand_count
}

fn part_b(lines: &Vec<Vec<(i32,i32)>>) -> i32 {
    let mut board : HashMap<(i32,i32),TileType> = HashMap::new();
    for line in lines {
        draw_line(line,&mut board);
    }
    let max_y = lines.iter().flatten().map(|x| x.1).max().unwrap();
    draw_line(&vec![(-100000,max_y+2),(100000,max_y+2)],&mut board);
    let mut sand_count = 0;
    let start_loc = (500,0);
    while drop_sand(&mut board, (500,0), max_y+2) {
        sand_count += 1;
        if board.get(&start_loc).is_some() {
            return sand_count;
        }

    }
    panic!("Shouldn't reach here!");
}

fn main() {
    let lines: Vec<Vec<(i32,i32)>> = read_lines("./day14.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            s.split(" -> ").map(|t| {
                let v : Vec<i32> = t.split(",").map(|x| x.parse::<i32>().unwrap()).collect();
                (*v.first().unwrap(),*v.last().unwrap())
            }).collect()
        })
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
