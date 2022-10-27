use aoc_2021::*;

fn bin_to_dec(s: &String) -> u64 {
    let mut res = 0;
    for ch in s.chars() {
        res *= 2;
        if ch == '1' {
            res += 1;
        }
    }
    res
}

fn part_a(all_bits: &Vec<String>) -> u64 {
    let n = all_bits[0].len();

    let mut zero_counts = vec![0; n];
    let mut one_counts = vec![0; n];

    for bits in all_bits {
        for (i, ch) in bits.chars().enumerate() {
            if ch == '0' {
                zero_counts[i] += 1;
            } else {
                assert_eq!(ch, '1');
                one_counts[i] += 1;
            }
        }
    }

    let mut gamma = String::new();
    let mut epsilon = String::new();

    for i in 0..n {
        if zero_counts[i] > one_counts[i] {
            gamma.push('0');
            epsilon.push('1');
        } else {
            assert!(zero_counts[i] != one_counts[i]);
            gamma.push('1');
            epsilon.push('0');
        }
    }
    bin_to_dec(&gamma) * bin_to_dec(&epsilon)
}

fn filter_by_bit(all_bits: &Vec<String>, bit_num: usize, most_or_least: bool) -> Vec<String> {
    let mut zero_count = 0;
    let mut one_count = 0;

    for bits in all_bits {
        for ch in bits.chars().skip(bit_num).take(1) {
            if ch == '0' {
                zero_count += 1;
            } else {
                assert_eq!(ch, '1');
                one_count += 1;
            }
        }
    }

    if zero_count == 0 || one_count == 0 {
        return all_bits.clone();
    }
    if zero_count > one_count {
        let mut ret_val: Vec<String> = Vec::new();
        for bits in all_bits {
            for ch in bits.chars().skip(bit_num).take(1) {
                if ch == '0' {
                    if most_or_least {
                        ret_val.push(bits.clone());
                    }
                } else {
                    assert_eq!(ch, '1');
                    if !most_or_least {
                        ret_val.push(bits.clone());
                    }
                }
            }
        }
        return ret_val;
    }

    let mut ret_val: Vec<String> = Vec::new();
    for bits in all_bits {
        for ch in bits.chars().skip(bit_num).take(1) {
            if ch == '1' {
                if most_or_least {
                    ret_val.push(bits.clone());
                }
            } else {
                assert_eq!(ch, '0');
                if !most_or_least {
                    ret_val.push(bits.clone());
                }
            }
        }
    }

    ret_val
}

fn part_b(all_bits: &Vec<String>) -> u64 {
    let mut all_bits_oxygen = all_bits.clone();
    {
        let mut bit_num = 0;
        while all_bits_oxygen.len() != 1 {
            all_bits_oxygen = filter_by_bit(&all_bits_oxygen, bit_num, true);
            bit_num += 1;
        }
    }

    let mut all_bits_co2 = all_bits.clone();
    {
        let mut bit_num = 0;
        while all_bits_co2.len() != 1 {
            all_bits_co2 = filter_by_bit(&all_bits_co2, bit_num, false);
            bit_num += 1;
        }
    }
    bin_to_dec(&all_bits_oxygen[0]) * bin_to_dec(&all_bits_co2[0])
}

fn main() {
    let bits: Vec<String> = read_lines("./day03.txt")
        .unwrap()
        .map(|l| l.unwrap())
        .collect();
    println!("Part A: {}", part_a(&bits));
    println!("Part B: {}", part_b(&bits));
}
