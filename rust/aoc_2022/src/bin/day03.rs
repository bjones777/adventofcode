use aoc_2022::*;

fn char_to_value(ch: char) -> i32 {
    let uc = (ch as i32) - ('A' as i32);
    if uc >= 0 && uc < 26 {
        return uc + 27;
    }
    let lc = (ch as i32) - ('a' as i32);
    if lc >= 0 && lc < 26 {
        return lc + 1;
    }
    panic!("can't map char {}", ch);
}

fn part_a(lines: &Vec<String>) -> i32 {
    use std::collections::HashSet;
    lines
        .iter()
        .map(|l| {
            let letters: Vec<char> = l.chars().collect();
            let n = letters.len();
            assert!(n % 2 == 0);
            let mut aset: HashSet<char> = HashSet::new();
            for i in 0..n / 2 {
                aset.insert(letters[i]);
            }
            for i in n / 2..n {
                if aset.contains(&letters[i]) {
                    return letters[i];
                }
            }
            panic!("No overlap!");
        })
        .map(|ch| char_to_value(ch))
        .sum()
}

fn part_b(lines: &Vec<String>) -> i32 {
    use std::collections::HashSet;
    lines
        .chunks(3)
        .map(|chunk| {
            if let [a, b, c] = &chunk[0..3] {
                let mut a_set: HashSet<char> = HashSet::new();
                for ch in a.chars() {
                    a_set.insert(ch);
                }
                let mut ab_set: HashSet<char> = HashSet::new();
                for ch in b.chars() {
                    if a_set.contains(&ch) {
                        ab_set.insert(ch);
                    }
                }
                for ch in c.chars() {
                    if ab_set.contains(&ch) {
                        return ch;
                    }
                }
                panic!("No overlap!");
            } else {
                panic!("non 0 remainder");
            }
        })
        .map(|ch| char_to_value(ch))
        .sum()
}

fn main() {
    let lines: Vec<String> = read_lines("./day03.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
