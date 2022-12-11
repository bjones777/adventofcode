use aoc_2022::*;

use regex::Regex;
use num_bigint::{BigInt, ToBigInt};

enum Operation {
    Mul(BigInt),
    Add(BigInt),
    Sqr,
}

struct Test {
    divisible_by: BigInt,
    if_true: usize,
    if_false: usize,
}

struct Monkey {
    index: usize,
    items: Vec<BigInt>,
    operation: Operation,
    test: Test,
}



fn parse_operation(lines: &Vec<String>, line_num: usize) -> Operation {
    let add_re = Regex::new(r".*Operation. new = old \+ (\d+)").unwrap();
    let mul_re = Regex::new(r".*Operation. new = old \* (\d+)").unwrap();
    let sqr_re = Regex::new(r".*Operation. new = old \* old").unwrap();
    let line = &lines[line_num];
    if let Some(cap) = add_re.captures(line) {
        return Operation::Add(cap.get(1).unwrap().as_str().parse::<BigInt>().unwrap());
    }
    else if let Some(cap) = mul_re.captures(line) {
        return Operation::Mul(cap.get(1).unwrap().as_str().parse::<BigInt>().unwrap());
    }
    else if let Some(_) = sqr_re.captures(line) {
        return Operation::Sqr;
    }
    panic!("Couldn't match operation");
}

fn parse_test(lines: &Vec<String>, line_num: usize) -> Test {
    let div_re = Regex::new(r".*Test. divisible by (\d+)").unwrap();
    let true_monkey_re = Regex::new(r".*If true. throw to monkey (\d+)").unwrap();
    let false_monkey_re = Regex::new(r".*If false. throw to monkey (\d+)").unwrap();

    Test { divisible_by: div_re.captures(&lines[line_num+0]).unwrap().get(1).unwrap().as_str().parse::<BigInt>().unwrap(), 
        if_true: true_monkey_re.captures(&lines[line_num+1]).unwrap().get(1).unwrap().as_str().parse::<usize>().unwrap(),
        if_false: false_monkey_re.captures(&lines[line_num+2]).unwrap().get(1).unwrap().as_str().parse::<usize>().unwrap(),}
}

fn parse_starting_items(lines: &Vec<String>, line_num: usize) -> Vec<BigInt> {
    let line = &lines[line_num];
    let prefix = "Starting items: ";
    if let Some(index) = line.as_str().find(prefix) {
        let substr = line.chars().skip(index + prefix.len()).collect::<String>();
        return substr.split(", ").map(|s| s.parse::<BigInt>().unwrap()).collect::<Vec<BigInt>>();
    }
    else {
        panic!("Could not find line");
    }
}

fn parse_index(lines: &Vec<String>, line_num: usize) -> usize {
    let index_re = Regex::new(r"Monkey (\d+).").unwrap();
    index_re.captures(&lines[line_num+0]).unwrap().get(1).unwrap().as_str().parse::<usize>().unwrap()
}

fn parse_monkey(lines: &Vec<String>, line_num: usize) -> Monkey {
    Monkey {
        index: parse_index(&lines, line_num),
        items: parse_starting_items(&lines, line_num+1),
        operation: parse_operation(&lines, line_num+2),
        test: parse_test(&lines, line_num+3),
    }
}

fn parse_monkeys(lines: &Vec<String>) -> Vec<Monkey> {
    let mut line_num = 0;
    let mut monkeys : Vec<Monkey> = Vec::new();
    loop {
        if line_num >= lines.len() {
            return monkeys;
        }
        let mut line = &lines[line_num];
        line_num += 1;
        while !line.as_str().starts_with("Monkey") {
            if line_num >= lines.len() {
                return monkeys;
            }
            line = &lines[line_num];
            line_num += 1;
        }
        monkeys.push(parse_monkey(&lines,line_num-1));
        line_num += 6;
    }
}

fn divide_worry_level(x: BigInt) -> BigInt {
    x / 3.to_bigint().unwrap()
}

fn part_a(lines: &Vec<String>) -> usize {
    let mut monkeys = parse_monkeys(&lines);
    let mut monkey_inspections = vec![0;monkeys.len()];
    for _ in 0..20 {
        let mut new_items: Vec<Vec<BigInt>> = vec![vec![]; monkeys.len()];
        for monkey in &mut monkeys {
            let mut items = monkey.items.clone();
            monkey.items.clear();
            items.append(&mut new_items[monkey.index]);
            monkey_inspections[monkey.index] += items.len();
            for item in &items {
                let worry_level = divide_worry_level(match &monkey.operation {
                    Operation::Add(x) => item + x,
                    Operation::Mul(x) => item * x,
                    Operation::Sqr => item*item ,
                });
                if worry_level.clone() % monkey.test.divisible_by.clone() == 0.to_bigint().unwrap() {
                    new_items[monkey.test.if_true].push(worry_level);
                }
                else {
                    new_items[monkey.test.if_false].push(worry_level);
                }
            }

        }

        // after the round, append any thrown items
        for monkey in &mut monkeys {
            monkey.items.append(&mut new_items[monkey.index]);
        }

    }
    monkey_inspections.sort();
    monkey_inspections[monkey_inspections.len()-1] * monkey_inspections[monkey_inspections.len()-2]
}

fn part_b(lines: &Vec<String>) -> usize {
    let mut monkeys = parse_monkeys(&lines);
    let max_mod : BigInt = monkeys.iter().map(|m| m.test.divisible_by.clone()).product();
    let mut monkey_inspections = vec![0;monkeys.len()];
    for _ in 0..10000 {
        let mut new_items: Vec<Vec<BigInt>> = vec![vec![]; monkeys.len()];
        for monkey in &mut monkeys {
            let mut items = monkey.items.clone();
            monkey.items.clear();
            items.append(&mut new_items[monkey.index]);
            monkey_inspections[monkey.index] += items.len();
            for item in &items {
                let worry_level = match &monkey.operation {
                    Operation::Add(x) => item + x,
                    Operation::Mul(x) => item * x,
                    Operation::Sqr => item*item,
                };
                if worry_level.clone() % monkey.test.divisible_by.clone() == 0.to_bigint().unwrap() {
                    new_items[monkey.test.if_true].push(worry_level % max_mod.clone());
                }
                else {
                    new_items[monkey.test.if_false].push(worry_level% max_mod.clone());
                }
            }

        }

        // after the round, append any thrown items
        for monkey in &mut monkeys {
            monkey.items.append(&mut new_items[monkey.index]);
        }

    }
    monkey_inspections.sort();
    monkey_inspections[monkey_inspections.len()-1] * monkey_inspections[monkey_inspections.len()-2]
}

fn main() {
    let lines: Vec<String> = read_lines("./day11.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .collect();
    
    println!("Part A: {}", part_a(&lines));

    // 27019168 too low
    println!("Part B: {}", part_b(&lines));
}
