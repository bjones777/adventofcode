use aoc_2022::*;

use std::collections::VecDeque;
#[derive(PartialEq)]
enum TileType {
    Start,
    End,
    Middle,
}

struct Tile {
    tile_type: TileType,
    height: i32,
}

fn find_start(lines: &Vec<Vec<Tile>>) -> (usize, usize) {
    let m = lines.len();
    let n = lines[0].len();
    for r in 0..m {
        for c in 0..n {
            if lines[r][c].tile_type == TileType::Start {
                return (r, c);
            }
        }
    }
    panic!("Start not found!");
}

fn find_shortest_path(
    visited: &mut Vec<Vec<usize>>,
    lines: &Vec<Vec<Tile>>,
    start: (usize, usize),
) -> usize {
    let m = lines.len();
    let n = lines[0].len();
    let mut queue: VecDeque<((usize, usize), usize)> = VecDeque::new();
    queue.push_back((start, 0));
    while !queue.is_empty() {
        let entry = queue.pop_front().unwrap();
        let loc = entry.0;
        let dist = entry.1;
        if lines[loc.0][loc.1].tile_type == TileType::End {
            return dist;
        }
        if visited[loc.0][loc.1] <= dist {
            continue;
        }
        visited[loc.0][loc.1] = dist;
        let new_dist = dist + 1;
        let current_height = lines[loc.0][loc.1].height;
        for i in 0..4 {
            if loc.0 == 0 && i == 0 {
                continue;
            }
            if loc.0 + 1 == m && i == 1 {
                continue;
            }
            if loc.1 == 0 && i == 2 {
                continue;
            }
            if loc.1 + 1 == n && i == 3 {
                continue;
            }
            let new_loc = match i {
                0 => (loc.0 - 1, loc.1),
                1 => (loc.0 + 1, loc.1),
                2 => (loc.0, loc.1 - 1),
                3 => (loc.0, loc.1 + 1),
                _ => panic!("didn't match..."),
            };
            if new_dist >= visited[new_loc.0][new_loc.1] {
                continue;
            }
            if lines[new_loc.0][new_loc.1].height > current_height + 1 {
                continue;
            }
            queue.push_back(((new_loc), new_dist));
        }
    }
    usize::MAX
}

fn part_a(lines: &Vec<Vec<Tile>>) -> usize {
    let start = find_start(lines);
    let m = lines.len();
    let n = lines[0].len();
    let mut visited = vec![vec![usize::MAX; n]; m];
    find_shortest_path(&mut visited, lines, start)
}

fn part_b(lines: &Vec<Vec<Tile>>) -> usize {
    let m = lines.len();
    let n = lines[0].len();
    let mut visited = vec![vec![usize::MAX; n]; m];
    let mut min_dist = usize::MAX;
    for r in 0..m {
        for c in 0..n {
            if lines[r][c].height == 0 {
                min_dist = usize::min(min_dist, find_shortest_path(&mut visited, lines, (r, c)));
            }
        }
    }
    min_dist
}

fn main() {
    let lines: Vec<Vec<Tile>> = read_lines("./day12.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            s.chars()
                .map(|ch| match ch {
                    'S' => Tile {
                        tile_type: TileType::Start,
                        height: 0,
                    },
                    'E' => Tile {
                        tile_type: TileType::End,
                        height: 25,
                    },
                    'a'..='z' => Tile {
                        tile_type: TileType::Middle,
                        height: (ch as i32) - ('a' as i32),
                    },
                    _ => panic!("unknown character"),
                })
                .collect()
        })
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
