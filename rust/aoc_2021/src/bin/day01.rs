use itertools::Itertools;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where
    P: AsRef<Path>,
{
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn part_a(meausrements: &Vec<i32>) -> i32 {
    meausrements
        .iter()
        .tuple_windows::<(_, _)>()
        .map(|(a, b)| if b > a { 1 } else { 0 })
        .sum::<i32>()
}

fn part_b(meausrements: &Vec<i32>) -> i32 {
    meausrements
        .iter()
        .tuple_windows::<(_, _, _)>()
        .map(|(a, b, c)| a + b + c)
        .tuple_windows::<(_, _)>()
        .map(|(a, b)| if b > a { 1 } else { 0 })
        .sum::<i32>()
}

fn main() {
    let measurements: Vec<i32> = read_lines("./day01.txt")
        .unwrap()
        .map(|l| l.unwrap().parse::<i32>().unwrap())
        .collect();
    println!("Part A: {}", part_a(&measurements));
    println!("Part B: {}", part_b(&measurements));
}
