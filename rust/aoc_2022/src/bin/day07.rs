use aoc_2022::*;

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

    fn traverse(&mut self, name: &String) -> &mut Entry {
        let parts: Vec<String> = name.split("/").map(|s| String::from(s)).collect();
        let mut current_entry = self;
        for i in 1..(parts.len() - 1) {
            let part = &parts[i];
            match current_entry.children.get_mut(part) {
                Some(entry) => current_entry = entry,
                None => panic!("Can't traverse"),
            }
        }
        current_entry
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

fn build_directory_structure(lines: &Vec<String>) -> Entry {
    let dir_re = Regex::new(r"^dir (.+)$").unwrap();
    let file_re = Regex::new(r"^(\d+) (.+)$").unwrap();
    let cd_cmd_re = Regex::new(r"^\$ cd (.+)$").unwrap();
    let ls_cmd_re = Regex::new(r"^\$ ls$").unwrap();
    let mut root_entry = Entry::new_dir();
    let mut current_dir = String::from("/");
    for line in lines {
        if let Some(cap) = dir_re.captures(line) {
            let cd = root_entry.traverse(&current_dir);
            let dir_name = cap.get(1).unwrap().as_str();
            cd.add_dir(String::from(dir_name));
        } else if let Some(cap) = file_re.captures(line) {
            let cd = root_entry.traverse(&current_dir);
            let file_name = cap.get(2).unwrap().as_str();
            let file_size = cap.get(1).unwrap().as_str().parse::<u64>().unwrap();
            cd.add_file(String::from(file_name), file_size);
        } else if let Some(cap) = cd_cmd_re.captures(line) {
            let dir_name = cap.get(1).unwrap().as_str();
            if dir_name == ".." {
                let mut parts: Vec<String> =
                    current_dir.split("/").map(|s| String::from(s)).collect();
                if !parts.is_empty() {
                    parts.pop();
                    parts.pop();
                }
                current_dir = parts.join("/") + "/";
            } else if dir_name == "/" {
                current_dir = String::from("/");
            } else {
                current_dir = current_dir + dir_name + "/";
            }
        } else if let Some(_) = ls_cmd_re.captures(line) {
        } else {
            print!("{}", line);
            panic!("Didn't match");
        }
    }
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
