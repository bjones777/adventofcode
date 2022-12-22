use aoc_2022::*;

use std::collections::HashMap;

#[derive(Clone, Copy)]
enum Dir {
    North,
    East,
    South,
    West,
}

fn turn_left(d : Dir) -> Dir {
    match d {
        Dir::North => Dir::West,
        Dir::West => Dir::South,
        Dir::South => Dir::East,
        Dir::East => Dir::North,
    }
}

fn turn_right(d : Dir) -> Dir {
    match d {
        Dir::North => Dir::East,
        Dir::West => Dir::North,
        Dir::South => Dir::West,
        Dir::East => Dir::South,
    }
}

fn get_delta(d: Dir) -> (i32,i32) {
    match d {
        Dir::North => (-1,0),
        Dir::West => (0,-1),
        Dir::South => (1,0),
        Dir::East => (0,1),
    }
}

fn get_facing(d : Dir) -> i32 {
    match d {
        Dir::North => 3,
        Dir::West => 2,
        Dir::South => 1,
        Dir::East => 0,
    }
}

fn get_direction(facing: usize) -> Dir {
    match facing {
        3 => Dir::North,
        2 => Dir::West,
        1 => Dir::South,
        0 => Dir::East,
        _ => panic!("Unknown facing {}", facing),
    }
}

fn get_opposite_direction(d : Dir) -> Dir {
    match d {
        Dir::North => Dir::South,
        Dir::West => Dir::East,
        Dir::South => Dir::North,
        Dir::East => Dir::West,
    }
}

fn add(a : (i32,i32), b : (i32,i32)) -> (i32,i32) {
    (a.0 + b.0, a.1 + b.1)
}

fn convert_coord(a : (usize,usize), delta: (i32,i32), row_ranges_of_columns : &Vec<(usize,usize)>,col_ranges_of_rows: &Vec<(usize,usize)>) -> (usize,usize) {
    if delta.0 < 0 && a.0 == col_ranges_of_rows[a.1].0 {
        return (col_ranges_of_rows[a.1].1,a.1);
    }
    if delta.1 < 0 && a.1 == row_ranges_of_columns[a.0].0 {
        return (a.0, row_ranges_of_columns[a.0].1);
    }
    if delta.0 > 0 && a.0 == col_ranges_of_rows[a.1].1 {
        return (col_ranges_of_rows[a.1].0,a.1);
    }
    if delta.1 > 0 && a.1 == row_ranges_of_columns[a.0].1 {
        return (a.0, row_ranges_of_columns[a.0].0);
    }
    let ai = (a.0 as i32, a.1 as i32);
    let ad = add(ai, delta);
    (ad.0 as usize, ad.1 as usize)
}

fn follow_path(monkey_map : &HashMap<(usize,usize),bool>,
    row_ranges_of_columns : &Vec<(usize,usize)>,
    col_ranges_of_rows: &Vec<(usize,usize)>,
    mut cl : (usize,usize),
    cur_dir : Dir,
    amount : i32) -> (usize,usize) {
    
    let delta = get_delta(cur_dir);
    for _ in 0..amount {
        let new_loc = convert_coord(cl, delta, row_ranges_of_columns, col_ranges_of_rows);
        if *monkey_map.get(&new_loc).unwrap() {
            return cl;
        }
        cl = new_loc;
    }
    cl
}

fn part_a(lines: &Vec<String>) -> i32 {
    let map_slice = &lines[0..lines.len()-2];
    let mut row_ranges_of_columns : Vec<(usize,usize)> = vec![ (usize::MAX,0); map_slice.len()];
    let max_columns = map_slice.iter().map(|s| s.chars().count()).max().unwrap();
    let mut col_ranges_of_rows : Vec<(usize,usize)> = vec![ (usize::MAX,0); max_columns];
    let mut monkey_map : HashMap<(usize,usize),bool> = HashMap::new();
    for (r,line) in map_slice.iter().enumerate() {
        for (c, ch) in line.chars().enumerate() {
            if ch == ' ' {
                continue;
            }
            let rr = row_ranges_of_columns[r];
            row_ranges_of_columns[r] = (usize::min(rr.0,c), usize::max(rr.1,c));
            monkey_map.insert((r,c), ch == '#');
            
            let cr = col_ranges_of_rows[c];
            col_ranges_of_rows[c] = (usize::min(cr.0,r), usize::max(cr.1,r));
        }
    }
    let mut cl = (0,row_ranges_of_columns[0].0);
    let mut cur_dir = Dir::East;
    println!("{:?}",cl);
    assert!(!monkey_map.get(&cl).unwrap());
    let path = lines.last().unwrap();
    let mut to_move = String::new();
    for path_char in path.chars() {
        match path_char {
            'L' => {
                if !to_move.is_empty() {
                    cl = follow_path(&monkey_map,&row_ranges_of_columns,&col_ranges_of_rows,cl,cur_dir,to_move.parse::<i32>().unwrap());
                }
                to_move.clear();
                cur_dir = turn_left(cur_dir);
            },
            'R' => {
                if !to_move.is_empty() {
                    cl = follow_path(&monkey_map,&row_ranges_of_columns,&col_ranges_of_rows,cl,cur_dir,to_move.parse::<i32>().unwrap());
                }
                to_move.clear();
                cur_dir = turn_right(cur_dir);
            }
            _ => {
                to_move.push(path_char);
            }
        }
    }
    if !to_move.is_empty() {
        cl = follow_path(&monkey_map,&row_ranges_of_columns,&col_ranges_of_rows,cl,cur_dir,to_move.parse::<i32>().unwrap());
    }

    ((cl.0+1) as i32) * 1000 + 4 * ((cl.1 + 1) as i32) + get_facing(cur_dir)
}


struct MonkeyMap {
    monkey_map: HashMap<(usize,usize),bool>,
    tile_size: usize,
    region_neighbors: Vec<Vec<i32>>,
}

impl MonkeyMap {
    fn get_local_region_coord(&self, a : (usize,usize)) -> (usize,usize) {
        (a.0 % self.tile_size, a.1 % self.tile_size)
    }

    fn get_global_coord(&self, a : (usize,usize), region: i32 ) -> (usize,usize) {
        if self.tile_size == 50 {
            match region {
                0 => (a.0+self.tile_size*0, a.1 + self.tile_size*1),
                1 => (a.0+self.tile_size*0, a.1 + self.tile_size*2),
                2 => (a.0+self.tile_size*1, a.1 + self.tile_size*1),
                3 => (a.0+self.tile_size*2, a.1 + self.tile_size*0),
                4 => (a.0+self.tile_size*2, a.1 + self.tile_size*1),
                5 => (a.0+self.tile_size*3, a.1 + self.tile_size*0),
                _ => panic!("Unknown region {}", region),
            }
        }
        else {
            match region {
                0 => (a.0+self.tile_size*0, a.1 + self.tile_size*2),
                1 => (a.0+self.tile_size*1, a.1 + self.tile_size*0),
                2 => (a.0+self.tile_size*1, a.1 + self.tile_size*1),
                3 => (a.0+self.tile_size*1, a.1 + self.tile_size*2),
                4 => (a.0+self.tile_size*2, a.1 + self.tile_size*2),
                5 => (a.0+self.tile_size*2, a.1 + self.tile_size*3),
                _ => panic!("Unknown region {}", region),
            }
        }
    }

    fn get_region_for_global_coord(&self, a : (usize,usize)) -> i32 {
        let r = a.0 / self.tile_size;
        let c = a.1 / self.tile_size;
        if self.tile_size == 50 {
            if r == 0 && c == 1 {
                return 0;
            }
            else if r == 0 && c == 2 {
                return 1;
            }
            else if r == 1 && c == 1 {
                return 2;
            }
            else if r == 2 && c == 0 {
                return 3;
            }
            else if r == 2 && c == 1 {
                return 4;
            }
            else if r == 3 && c == 0 {
                return 5;
            }
            panic!("In unknown region!");
        }
        if r == 0 && c == 2 {
            return 0;
        }
        else if r == 1 && c == 0 {
            return 1;
        }
        else if r == 1 && c == 1 {
            return 2;
        }
        else if r == 1 && c == 2 {
            return 3;
        }
        else if r == 2 && c == 2 {
            return 4;
        }
        else if r == 2 && c == 3 {
            return 5;
        }
        panic!("In unknown region!");
    }

    fn follow_path(&self,mut cl : (usize,usize), mut cur_dir : Dir,amount : i32 ) -> ((usize,usize),Dir) {
        for _ in 0..amount {
            let (new_loc, new_dir) = self.convert_coord(cl, cur_dir);
            if *self.monkey_map.get(&new_loc).unwrap() {
                return (cl,cur_dir);
            }
            cl = new_loc;
            cur_dir = new_dir;
        }
        (cl, cur_dir)
    }

    fn convert_coord(&self, a: (usize,usize), cur_dir: Dir) -> ((usize,usize),Dir) {
        let delta = get_delta(cur_dir);
        let local_coord = self.get_local_region_coord(a);
        if delta.0 < 0 && local_coord.0 == 0  {
            return self.adjust_for_region(a,cur_dir);
        }
        if delta.1 < 0 && local_coord.1 == 0 {
            return self.adjust_for_region(a,cur_dir);
        }
        if delta.0 > 0 && local_coord.0 + 1 == self.tile_size {
            return self.adjust_for_region(a,cur_dir);
        }
        if delta.1 > 0 && local_coord.1 + 1 == self.tile_size {
            return self.adjust_for_region(a,cur_dir);
        }
        let ai = (a.0 as i32, a.1 as i32);
        let ad = add(ai, delta);
        ((ad.0 as usize, ad.1 as usize), cur_dir)
    }

    fn adjust_for_region(&self, a: (usize,usize), cur_dir: Dir) -> ((usize,usize),Dir) {
        let current_region = self.get_region_for_global_coord(a);
        let current_region_index = current_region as usize;
        let facing_index = get_facing(cur_dir) as usize;

        let new_region = self.region_neighbors[current_region_index][facing_index];

        let side = self.find_direction_to_region(new_region, current_region);
        let new_direction = get_opposite_direction(side);

        let local_coord = self.get_local_region_coord(a);
        let n = self.tile_size-1;

        let new_local_coord = match cur_dir {
            Dir::North => match side {
                Dir::North => (0               ,n-local_coord.1),
                Dir::West  => (local_coord.1  , 0),
                Dir::South => (n              ,local_coord.1),
                Dir::East  => (n-local_coord.1, n)
            },
            Dir::West => match side {
                Dir::North => (0               ,local_coord.0),
                Dir::West  => (n-local_coord.0 , 0),
                Dir::South => (n              , n-local_coord.0),
                Dir::East  => (local_coord.0  , n),
            },
            Dir::South =>  match side {
                Dir::North => (0               ,local_coord.1),
                Dir::West  => (n-local_coord.1 ,0),
                Dir::South => (n              , n-local_coord.1),
                Dir::East  => (local_coord.1  , n),
            },
            Dir::East =>  match side {
                Dir::North => (0               ,n-local_coord.0),
                Dir::West  => (local_coord.0   ,0),
                Dir::South => (n              , local_coord.0),
                Dir::East  => (n-local_coord.0, n),
            },
        };
        (self.get_global_coord(new_local_coord, new_region), new_direction)

    }

    fn find_direction_to_region(&self, region1 : i32, region2: i32) -> Dir {
        for t in self.region_neighbors[region1 as usize].iter().enumerate() {
            if *t.1 == region2 {
                return get_direction(t.0);
            }
        }
        panic!("couldn't find dir for {} to {}", region1, region2);
    }
    
    
}

fn build_region_neighbors(tile_size: usize) -> Vec<Vec<i32>> {
    if tile_size == 50 {
        //          E, S, W, N
        vec![ vec![ 1, 2, 3, 5] , // 0
              vec![ 4, 2, 0, 5] , // 1
              vec![ 1, 4, 3, 0] , // 2
              vec![ 4, 5, 0, 2] , // 3
              vec![ 1, 5, 3, 2] , // 4
              vec![ 4, 1, 0, 3] , // 5
        ]
    }
    else {
        //          E, S, W, N
        vec![ vec![ 5, 3, 2, 1] , // 0
              vec![ 2, 4, 5, 0] , // 1
              vec![ 3, 4, 1, 0] , // 2
              vec![ 5, 4, 2, 0] , // 3
              vec![ 5, 1, 2, 3] , // 4
              vec![ 0, 1, 4, 3] , // 5
        ]
    }
}

fn part_b(lines: &Vec<String>) -> i32 {
    let map_slice = &lines[0..lines.len()-2];
    let mut row_ranges_of_columns : Vec<(usize,usize)> = vec![ (usize::MAX,0); map_slice.len()];
    let max_columns = map_slice.iter().map(|s| s.chars().count()).max().unwrap();
    let mut col_ranges_of_rows : Vec<(usize,usize)> = vec![ (usize::MAX,0); max_columns];
    let mut monkey_map : HashMap<(usize,usize),bool> = HashMap::new();
    for (r,line) in map_slice.iter().enumerate() {
        for (c, ch) in line.chars().enumerate() {
            if ch == ' ' {
                continue;
            }
            let rr = row_ranges_of_columns[r];
            row_ranges_of_columns[r] = (usize::min(rr.0,c), usize::max(rr.1,c));
            monkey_map.insert((r,c), ch == '#');
            
            let cr = col_ranges_of_rows[c];
            col_ranges_of_rows[c] = (usize::min(cr.0,r), usize::max(cr.1,r));
        }
    }
    let tile_size = if lines.len() < 50 { 4 } else { 50 };
    let cube_monkey_map = MonkeyMap {
        monkey_map: monkey_map,
        tile_size: tile_size,
        region_neighbors: build_region_neighbors(tile_size),

    };
    let mut cl = (0,row_ranges_of_columns[0].0);
    let mut cur_dir = Dir::East;
    println!("{:?}",cl);
    let path = lines.last().unwrap();
    let mut to_move = String::new();
    for path_char in path.chars() {
        match path_char {
            'L' => {
                if !to_move.is_empty() {
                    (cl,cur_dir) = cube_monkey_map.follow_path(cl,cur_dir,to_move.parse::<i32>().unwrap());
                }
                to_move.clear();
                cur_dir = turn_left(cur_dir);
            },
            'R' => {
                if !to_move.is_empty() {
                    (cl,cur_dir) = cube_monkey_map.follow_path(cl,cur_dir,to_move.parse::<i32>().unwrap());
                }
                to_move.clear();
                cur_dir = turn_right(cur_dir);
            }
            _ => {
                to_move.push(path_char);
            }
        }
    }
    if !to_move.is_empty() {
        (cl,cur_dir) = cube_monkey_map.follow_path(cl,cur_dir,to_move.parse::<i32>().unwrap());
    }

    ((cl.0+1) as i32) * 1000 + 4 * ((cl.1 + 1) as i32) + get_facing(cur_dir)
}

fn main() {
    let lines: Vec<String> = read_lines("./day22.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
