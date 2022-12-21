use aoc_2022::*;

use regex::Regex;
use std::collections::HashMap;

enum Operation {
    Add,
    Multiply,
    Subtract,
    Divide,
}
enum MonkeyType {
    Op(String, Operation, String),
    Number(i64),
}

struct Monkey {
    name: String,
    behavior: MonkeyType,
}

fn eval(
    name: &String,
    name_to_index: &HashMap<String, usize>,
    memo: &mut HashMap<String, i64>,
    monkeys: &Vec<Monkey>,
) -> i64 {
    if let Some(v) = memo.get(name) {
        return *v;
    }
    let index = *name_to_index.get(name).unwrap();
    let v = match &monkeys[index].behavior {
        MonkeyType::Number(v) => *v,
        MonkeyType::Op(a, o, b) => match o {
            Operation::Add => {
                eval(a, name_to_index, memo, monkeys) + eval(b, name_to_index, memo, monkeys)
            }
            Operation::Subtract => {
                eval(a, name_to_index, memo, monkeys) - eval(b, name_to_index, memo, monkeys)
            }
            Operation::Multiply => {
                eval(a, name_to_index, memo, monkeys) * eval(b, name_to_index, memo, monkeys)
            }
            Operation::Divide => {
                eval(a, name_to_index, memo, monkeys) / eval(b, name_to_index, memo, monkeys)
            }
        },
    };
    memo.insert(name.clone(), v);
    v
}

fn part_a(monkeys: &Vec<Monkey>) -> i64 {
    let mut name_to_index: HashMap<String, usize> = HashMap::new();
    for t in monkeys.iter().enumerate() {
        name_to_index.insert(t.1.name.clone(), t.0);
    }
    let mut memo: HashMap<String, i64> = HashMap::new();
    eval(&"root".to_string(), &name_to_index, &mut memo, monkeys)
}

fn part_b(monkeys: &Vec<Monkey>) -> i64 {
    let mut name_to_index: HashMap<String, usize> = HashMap::new();
    for t in monkeys.iter().enumerate() {
        name_to_index.insert(t.1.name.clone(), t.0);
    }
    let root_monkey = *name_to_index.get(&"root".to_string()).unwrap();
    let (a, b) = match &monkeys[root_monkey].behavior {
        MonkeyType::Op(a, _, b) => (a.clone(), b.clone()),
        MonkeyType::Number(_) => panic!("Root can't be a number"),
    };
    let mut lb = -10_000_000_000_000i64;
    let mut ub = 10_000_000_000_000i64;
    while lb <= ub {
        let x = (ub - lb) / 2 + lb;
        let mut memo: HashMap<String, i64> = HashMap::new();
        memo.insert("humn".to_string(), x);
        let ax = eval(&a, &name_to_index, &mut memo, &monkeys);
        let bx = eval(&b, &name_to_index, &mut memo, &monkeys);
        if ax == bx {
            return x;
        }
        if ax > bx {
            lb = x + 1;
        } else {
            ub = x - 1;
        }
    }
    panic!("Not found!");
}

fn to_op(s: &str) -> Operation {
    match s {
        "+" => Operation::Add,
        "-" => Operation::Subtract,
        "*" => Operation::Multiply,
        "/" => Operation::Divide,
        _ => panic!("Couldn't covert {} to operation", s),
    }
}

fn main() {
    let number_re = Regex::new(r"^(\w+). (\d+)$").unwrap();
    let op_re = Regex::new(r"^(\w+). (\w+) (.) (\w+)$").unwrap();
    let lines: Vec<Monkey> = read_lines("./day21.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            if let Some(cap) = number_re.captures(&s) {
                return Monkey {
                    name: cap.get(1).unwrap().as_str().to_string(),
                    behavior: MonkeyType::Number(
                        cap.get(2).unwrap().as_str().parse::<i64>().unwrap(),
                    ),
                };
            } else if let Some(cap) = op_re.captures(&s) {
                return Monkey {
                    name: cap.get(1).unwrap().as_str().to_string(),
                    behavior: MonkeyType::Op(
                        cap.get(2).unwrap().as_str().to_string(),
                        to_op(cap.get(3).unwrap().as_str()),
                        cap.get(4).unwrap().as_str().to_string(),
                    ),
                };
            }
            panic!("Couldn't match line {}", s)
        })
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
