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
    panic!("can't map char");
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
    let num_groups = lines.len() / 3;
    assert!(lines.len() % 3 == 0);
    let mut sum = 0;
    for i in 0..num_groups {
        let a = &lines[i * 3 + 0];
        let b = &lines[i * 3 + 1];
        let c = &lines[i * 3 + 2];

        let mut aset: HashSet<char> = HashSet::new();
        for ch in a.chars() {
            aset.insert(ch);
        }
        let mut bset: HashSet<char> = HashSet::new();
        for ch in b.chars() {
            if aset.contains(&ch) {
                bset.insert(ch);
            }
        }
        let mut was_found = false;
        for ch in c.chars() {
            if bset.contains(&ch) {
                sum += char_to_value(ch);
                was_found = true;
                break;
            }
        }
        assert!(was_found);
    }
    sum
}

fn main() {
    let lines: Vec<String> = read_lines("./day03.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
