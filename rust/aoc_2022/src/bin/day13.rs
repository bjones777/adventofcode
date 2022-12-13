use aoc_2022::*;

use std::cmp::Ordering;

enum Packet {
    Int(i32),
    List(Vec<Packet>),
}

#[derive(PartialEq)]
enum Token {
    Digit,
    Open,
    Close,
    Comma,
}

fn consume(line: &Vec<char>, index: usize, ch: char) -> usize {
    assert!(line[index] == ch);
    index + 1
}

fn peek_next_token(line: &Vec<char>, index: usize) -> Token {
    match line[index] {
        '[' => Token::Open,
        ']' => Token::Close,
        ',' => Token::Comma,
        '0'..='9' => Token::Digit,
        _ => panic!("Unknown character"),
    }
}

fn parse_number(line: &Vec<char>, mut index: usize) -> (i32, usize) {
    let mut digits = String::new();
    while peek_next_token(line, index) == Token::Digit {
        let ch = line[index];
        index += 1;
        digits.push(ch);
    }
    (digits.parse::<i32>().unwrap(), index)
}

fn parse_packet(line: &Vec<char>, start_index: usize) -> (Packet, usize) {
    let mut index = start_index;
    index = consume(line, index, '[');
    let mut packets: Vec<Packet> = Vec::new();
    loop {
        match peek_next_token(line, index) {
            Token::Close => {
                index = consume(line, index, ']');
                return (Packet::List(packets), index);
            }
            Token::Open => {
                let t = parse_packet(line, index);
                packets.push(t.0);
                index = t.1;
            }
            Token::Digit => {
                let t = parse_number(line, index);
                packets.push(Packet::Int(t.0));
                index = t.1;
            }
            Token::Comma => {
                index = consume(line, index, ',');
            }
        }
    }
}

fn in_right_order(p1: &Packet, p2: &Packet) -> Ordering {
    match p1 {
        Packet::Int(v1) => match p2 {
            Packet::Int(v2) => {
                if *v1 < *v2 {
                    return Ordering::Less;
                }
                if *v1 > *v2 {
                    return Ordering::Greater;
                }
                return Ordering::Equal;
            }
            Packet::List(_) => {
                return in_right_order(&Packet::List(vec![Packet::Int(*v1)]), p2);
            }
        },
        Packet::List(v1) => match p2 {
            Packet::Int(v2) => {
                return in_right_order(p1, &Packet::List(vec![Packet::Int(*v2)]));
            }
            Packet::List(v2) => {
                let n = usize::min(v1.len(), v2.len());
                for i in 0..n {
                    let ord = in_right_order(&v1[i], &v2[i]);
                    if ord != Ordering::Equal {
                        return ord;
                    }
                }
                if v1.len() < v2.len() {
                    return Ordering::Less;
                }
                if v1.len() > v2.len() {
                    return Ordering::Greater;
                }
                return Ordering::Equal;
            }
        },
    }
}

fn part_a(lines: &Vec<String>) -> usize {
    let num_pairs = (lines.len() + 1) / 3;
    let mut sum = 0;
    for i in 0..num_pairs {
        let i1 = if i == 0 { 0 } else { 3 * i };
        let i2 = i1 + 1;
        let packet_chars_1: Vec<char> = lines[i1].chars().collect();
        let packet_chars_2: Vec<char> = lines[i2].chars().collect();
        let p1 = parse_packet(&packet_chars_1, 0).0;
        let p2 = parse_packet(&packet_chars_2, 0).0;
        if in_right_order(&p1, &p2) == Ordering::Less {
            sum += i + 1
        }
    }
    sum
}

fn part_b(lines: &Vec<String>) -> usize {
    let num_pairs = (lines.len() + 1) / 3;
    let mut packets: Vec<Packet> = Vec::new();
    packets.push(Packet::List(vec![Packet::List(vec![Packet::Int(2)])]));
    packets.push(Packet::List(vec![Packet::List(vec![Packet::Int(6)])]));
    for i in 0..num_pairs {
        let i1 = if i == 0 { 0 } else { 3 * i };
        let i2 = i1 + 1;
        let packet_chars_1: Vec<char> = lines[i1].chars().collect();
        let packet_chars_2: Vec<char> = lines[i2].chars().collect();
        packets.push(parse_packet(&packet_chars_1, 0).0);
        packets.push(parse_packet(&packet_chars_2, 0).0);
    }
    packets.sort_by(|a, b| in_right_order(a, b));
    let i1 = packets
        .iter()
        .position(|a| {
            in_right_order(a, &Packet::List(vec![Packet::List(vec![Packet::Int(2)])]))
                == Ordering::Equal
        })
        .unwrap()
        + 1;
    let i2 = packets
        .iter()
        .position(|a| {
            in_right_order(a, &Packet::List(vec![Packet::List(vec![Packet::Int(6)])]))
                == Ordering::Equal
        })
        .unwrap()
        + 1;
    i1 * i2
}

fn main() {
    let lines: Vec<String> = read_lines("./day13.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
