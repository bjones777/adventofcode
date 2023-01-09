use aoc_2022::*;

use lazy_static::lazy_static;
use regex::Regex;
use std::collections::HashMap;

struct Entry {
    size: u64,
    children: HashMap<String, Entry>,
    is_file: bool,
}

impl Entry {
    fn new_file(size: u64) -> Entry {
        Entry {
            size: size,
            children: HashMap::new(),
            is_file: true,
        }
    }

    fn new_dir() -> Entry {
        Entry {
            size: 0,
            children: HashMap::new(),
            is_file: false,
        }
    }

    fn add_dir(&mut self, name: String) {
        let new_dir = Entry::new_dir();
        self.children.insert(name, new_dir);
    }

    fn add_file(&mut self, name: String, size: u64) {
        let new_file = Entry::new_file(size);
        self.children.insert(name, new_file);
    }

    fn get_total_size_and_children(&self) -> Vec<(u64, bool)> {
        if self.is_file {
            return vec![(self.size, true)];
        }
        let mut total_sizes: Vec<(u64, bool)> = Vec::new();
        let mut total_size: u64 = 0;
        for child in &self.children {
            let mut child_sizes = child.1.get_total_size_and_children();
            total_size += child_sizes.last().unwrap().0;
            total_sizes.append(&mut child_sizes);
        }
        total_sizes.push((total_size, false));
        total_sizes
    }
}

fn build_dir_structure(lines: &Vec<String>, line_index: &mut usize, cd: &mut Entry, depth: usize) {
    lazy_static! {
        static ref DIR_RE: Regex = Regex::new(r"^dir (.+)$").unwrap();
        static ref FILE_RE: Regex = Regex::new(r"^(\d+) (.+)$").unwrap();
        static ref CD_CMD_RE: Regex = Regex::new(r"^\$ cd (.+)$").unwrap();
        static ref LS_CMD_RE: Regex = Regex::new(r"^\$ ls$").unwrap();
    }

    while *line_index < lines.len() {
        let line = &lines[*line_index];
        *line_index += 1;
        if let Some(cap) = DIR_RE.captures(line) {
            let dir_name = cap.get(1).unwrap().as_str();
            cd.add_dir(String::from(dir_name));
        } else if let Some(cap) = FILE_RE.captures(line) {
            let file_name = cap.get(2).unwrap().as_str();
            let file_size = cap.get(1).unwrap().as_str().parse::<u64>().unwrap();
            cd.add_file(String::from(file_name), file_size);
        } else if let Some(cap) = CD_CMD_RE.captures(line) {
            let dir_name = cap.get(1).unwrap().as_str();
            if dir_name == ".." {
                return;
            } else if dir_name == "/" {
                assert!(depth == 0);
            } else {
                build_dir_structure(
                    lines,
                    line_index,
                    cd.children.get_mut(dir_name).unwrap(),
                    depth + 1,
                );
            }
        } else if let Some(_) = LS_CMD_RE.captures(line) {
        } else {
            print!("{}", line);
            panic!("Didn't match");
        }
    }
}
fn build_directory_structure(lines: &Vec<String>) -> Entry {
    let mut root_entry = Entry::new_dir();
    build_dir_structure(lines, &mut 0, &mut root_entry, 0);
    root_entry
}

fn part_a(total_sizes: &Vec<(u64, bool)>) -> u64 {
    total_sizes
        .iter()
        .map(|x| if !x.1 && x.0 <= 100000 { x.0 } else { 0 })
        .sum()
}

fn part_b(total_sizes: &Vec<(u64, bool)>) -> i64 {
    let used_space = total_sizes.last().unwrap().0 as i64;
    let free_space = 70000000i64 - used_space;
    let needed_space = 30000000 - free_space;
    total_sizes
        .iter()
        .filter(|x| !x.1 && (x.0 as i64) >= needed_space)
        .map(|x| x.0 as i64)
        .min()
        .unwrap()
}

fn main() {
    let lines: Vec<String> = read_lines("./day07.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .collect();
    let root_entry = build_directory_structure(&lines);
    let total_sizes = root_entry.get_total_size_and_children();
    println!("Part A: {}", part_a(&total_sizes));
    println!("Part B: {}", part_b(&total_sizes));
}
