use aoc_2022::*;
use num_traits::pow;

fn from_snafu(str: &String) -> i64 {
    str.chars()
        .rev()
        .enumerate()
        .map(|(place, ch)| {
            let p = pow(5 as i64, place);
            match ch {
                '2' => 2i64 * p,
                '1' => 1i64 * p,
                '0' => 0,
                '-' => -1i64 * p,
                '=' => -2i64 * p,
                _ => panic!("Unknown char {}", ch),
            }
        })
        .sum()
}

fn to_snafu(mut v: i64) -> String {
    let mut ret_val = String::new();
    while v != 0 {
        let mut dv = v / 5;
        let rv = v % 5;
        match rv {
            0 => ret_val.push('0'),
            1 => ret_val.push('1'),
            2 => ret_val.push('2'),
            3 => {
                dv += 1;
                ret_val.push('=');
            }
            4 => {
                dv += 1;
                ret_val.push('-');
            }
            _ => panic!("Shouldn't hit this, div 5 above"),
        }
        v = dv;
    }
    if ret_val.is_empty() {
        ret_val.push('0');
    }
    String::from_iter(ret_val.chars().rev())
}

fn main() {
    let total: i64 = read_lines("./day25.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| from_snafu(&s))
        .sum();
    println!("Part A: {}", to_snafu(total));
}
