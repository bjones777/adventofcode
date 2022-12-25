use aoc_2022::*;

use std::collections::{VecDeque, HashSet};

#[derive(Clone, Copy, PartialEq, Eq,)]
enum TileType {
    Empty,
    Wall,
}


#[derive(Clone, Copy)]
enum Dir {
    North,
    East,
    South,
    West,
}

#[derive(Clone, Copy)]
struct Blizzard {
    loc: (usize,usize),
    dir: Dir,
}

fn move_blizzards(board : &Vec<Vec<TileType>>, blizzards: &Vec<Blizzard>) -> Vec<Blizzard> {
    let mut moved_blizzards : Vec<Blizzard> = Vec::new();
    for blizzard in blizzards {
        match blizzard.dir {
            Dir::East => {
                if board[blizzard.loc.0][blizzard.loc.1+1] == TileType::Wall {
                    moved_blizzards.push(Blizzard { loc: (blizzard.loc.0,1), dir: blizzard.dir });
                }
                else {
                    moved_blizzards.push(Blizzard { loc: (blizzard.loc.0, blizzard.loc.1+1), dir: blizzard.dir });
                }
            },
            Dir::West =>  {
                if board[blizzard.loc.0][blizzard.loc.1-1] == TileType::Wall {
                    moved_blizzards.push(Blizzard { loc: (blizzard.loc.0,board[blizzard.loc.0].len()-2), dir: blizzard.dir });
                }
                else {
                    moved_blizzards.push(Blizzard { loc: (blizzard.loc.0, blizzard.loc.1-1), dir: blizzard.dir });
                }
            },
            Dir::South => {
                if board[blizzard.loc.0+1][blizzard.loc.1] == TileType::Wall {
                    moved_blizzards.push(Blizzard { loc: (2,blizzard.loc.1), dir: blizzard.dir });
                }
                else {
                    moved_blizzards.push(Blizzard { loc: (blizzard.loc.0+1, blizzard.loc.1), dir: blizzard.dir });
                }
            },
            Dir::North => {
                if board[blizzard.loc.0-1][blizzard.loc.1] == TileType::Wall {
                    moved_blizzards.push(Blizzard { loc: (board.len()-3,blizzard.loc.1), dir: blizzard.dir });
                }
                else {
                    moved_blizzards.push(Blizzard { loc: (blizzard.loc.0-1, blizzard.loc.1), dir: blizzard.dir });
                }
            },
        }
    }
    moved_blizzards
}

fn enqueue(in_q: &mut HashSet<((usize, usize), usize)>,queue: &mut VecDeque<((usize, usize), usize)>,new_state: ((usize, usize), usize)) {
    if in_q.contains(&new_state) {
        return;
    }
    queue.push_back(new_state);
    in_q.insert(new_state);
}

fn is_loc_safe(loc: (usize,usize), board : &Vec<Vec<TileType>>, blizzards_at_round: &mut Vec<Vec<Blizzard>>, blizzard_locs_at_round: &mut Vec<HashSet<(usize,usize)>>,round : usize) -> bool {
    while round+1 > blizzards_at_round.len() {
        blizzards_at_round.push(move_blizzards(board, blizzards_at_round.last().unwrap()))
    }
    while blizzard_locs_at_round.len() < blizzards_at_round.len() {
        let mut locs : HashSet<(usize,usize)> = HashSet::new();
        for vb in blizzards_at_round[blizzard_locs_at_round.len()].iter() {
            locs.insert(vb.loc);
        }
        blizzard_locs_at_round.push(locs);
    }

    let ret_val = !blizzard_locs_at_round[round].contains(&loc);
    //println!("nope {}", ret_val);
    ret_val
}

fn get_rounds_to_reach_dest(start_loc : (usize,usize),
     end_loc: (usize,usize), from_round: usize, blizzards_at_round: &mut Vec<Vec<Blizzard>>, blizzard_locs_at_round: &mut Vec<HashSet<(usize,usize)>>
    , board : &Vec<Vec<TileType>>) -> usize {
    let mut in_q: HashSet<((usize, usize), usize)> = HashSet::new();
    let mut queue: VecDeque<((usize, usize), usize)> = VecDeque::new();
    enqueue(&mut in_q, &mut queue, (start_loc,from_round));
    while !queue.is_empty() {
        let state = queue.pop_front().unwrap();
        if state.0 == end_loc {
            return state.1 + 1;
        }
        let loc = state.0;
        if is_loc_safe(state.0,&board,blizzards_at_round, blizzard_locs_at_round, state.1+1) {
            enqueue(&mut in_q, &mut queue,(state.0,state.1+1));
        }
        if board[loc.0-1][loc.1] == TileType::Empty && is_loc_safe((loc.0-1,loc.1),&board,blizzards_at_round, blizzard_locs_at_round, state.1+1) {
            enqueue(&mut in_q, &mut queue,((loc.0-1,loc.1), state.1+1));
        }
        if board[loc.0+1][loc.1] == TileType::Empty && is_loc_safe((loc.0+1,loc.1),&board,blizzards_at_round, blizzard_locs_at_round, state.1+1) {
            enqueue(&mut in_q, &mut queue,((loc.0+1,loc.1), state.1+1));
        }
        if board[loc.0][loc.1+1] == TileType::Empty && is_loc_safe((loc.0,loc.1+1),&board,blizzards_at_round, blizzard_locs_at_round, state.1+1) {
            enqueue(&mut in_q, &mut queue,((loc.0,loc.1+1), state.1+1));
        }
        if board[loc.0][loc.1-1] == TileType::Empty && is_loc_safe((loc.0,loc.1-1),&board,blizzards_at_round, blizzard_locs_at_round, state.1+1) {
            enqueue(&mut in_q, &mut queue,((loc.0,loc.1-1), state.1+1));
        }
    }
    panic!("Couldn't find path");
}

fn part_a(lines: &Vec<String>) -> usize {
    let num_rows = lines.len()+2;
    let num_cols = lines.first().unwrap().len();
    let mut board : Vec<Vec<TileType>> = vec![vec![TileType::Empty; num_cols]; num_rows];
    for c in 0..num_cols {
        board[0][c] = TileType::Wall;
        board[num_rows-1][c] = TileType::Wall;
    }
    let mut blizzards : Vec<Blizzard> = vec![];
    for r in 0..(num_rows-2) {
        for (c,ch) in lines[r].chars().enumerate() {
            match ch {
                '#' => board[r+1][c] = TileType::Wall,
                '^' => blizzards.push(Blizzard { loc: (r+1, c), dir: Dir::North }),
                '>' => blizzards.push(Blizzard { loc: (r+1, c), dir: Dir::East }),
                '<' => blizzards.push(Blizzard { loc: (r+1, c), dir: Dir::West }),
                'v' => blizzards.push(Blizzard { loc: (r+1, c), dir: Dir::South }),
                '.' => {},
                _ => panic!("Unknown char {}", ch),
            }
        }
    }

    let start_loc = (1,1);
    let end_loc = (lines.len()-1,lines.first().unwrap().len()-2);
    
    let mut blizzards_at_round:Vec<Vec<Blizzard>> = Vec::new();
    blizzards_at_round.push(blizzards);


    let mut blizzard_locs_at_round: Vec<HashSet<(usize,usize)>> = vec![];
    
    get_rounds_to_reach_dest(start_loc,end_loc,0,&mut blizzards_at_round, &mut blizzard_locs_at_round, &board)
}

fn part_b(lines: &Vec<String>) -> usize {
    let num_rows = lines.len()+2;
    let num_cols = lines.first().unwrap().len();
    let mut board : Vec<Vec<TileType>> = vec![vec![TileType::Empty; num_cols]; num_rows];
    for c in 0..num_cols {
        board[0][c] = TileType::Wall;
        board[num_rows-1][c] = TileType::Wall;
    }
    let mut blizzards : Vec<Blizzard> = vec![];
    for r in 0..(num_rows-2) {
        for (c,ch) in lines[r].chars().enumerate() {
            match ch {
                '#' => board[r+1][c] = TileType::Wall,
                '^' => blizzards.push(Blizzard { loc: (r+1, c), dir: Dir::North }),
                '>' => blizzards.push(Blizzard { loc: (r+1, c), dir: Dir::East }),
                '<' => blizzards.push(Blizzard { loc: (r+1, c), dir: Dir::West }),
                'v' => blizzards.push(Blizzard { loc: (r+1, c), dir: Dir::South }),
                '.' => {},
                _ => panic!("Unknown char {}", ch),
            }
        }
    }

    let mut blizzards_at_round:Vec<Vec<Blizzard>> = Vec::new();
    blizzards_at_round.push(blizzards);


    let mut blizzard_locs_at_round: Vec<HashSet<(usize,usize)>> = vec![];

    let start_loc = (1,1);
    let end_loc = (lines.len()-1,lines.first().unwrap().len()-2);
    
    let rounds_to_end = get_rounds_to_reach_dest(start_loc,end_loc,0,&mut blizzards_at_round, &mut blizzard_locs_at_round, &board);
    let rounds_to_start =  get_rounds_to_reach_dest(end_loc, start_loc,rounds_to_end,&mut blizzards_at_round, &mut blizzard_locs_at_round, &board);
    get_rounds_to_reach_dest(start_loc,end_loc,rounds_to_start+1,&mut blizzards_at_round, &mut blizzard_locs_at_round, &board)
}

fn main() {
    let lines: Vec<String> = read_lines("./day24.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .collect();
    
    println!("Part A: {}", part_a(&lines));

    // 722 too low
    println!("Part B: {}", part_b(&lines));
}
