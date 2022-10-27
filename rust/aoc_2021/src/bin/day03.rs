use aoc_2021::*;

fn bin_to_dec(v: &Vec<u8>) -> u64 {
    let mut res = 0;
    for x in v {
        res *= 2;
        res += *x as u64;
    }
    res
}

fn part_a(all_bits: &Vec<Vec<u8>>) -> u64 {
    let n = all_bits[0].len();

    let mut counts = vec![vec![0; n], vec![0; n]];

    for bits in all_bits {
        for (i, x) in bits.iter().enumerate() {
            counts[*x as usize][i] += 1;
        }
    }

    let mut gamma = Vec::new();
    let mut epsilon = Vec::new();

    for i in 0..n {
        if counts[0][i] > counts[1][i] {
            gamma.push(0_u8);
            epsilon.push(1_u8);
        } else {
            assert!(counts[1][i] > counts[0][i]);
            gamma.push(1_u8);
            epsilon.push(0_u8);
        }
    }
    bin_to_dec(&gamma) * bin_to_dec(&epsilon)
}

fn filter_by_bit(all_bits: Vec<Vec<u8>>, bit_num: usize, most_or_least: bool) -> Vec<Vec<u8>> {
    let mut counts = vec![0; 2];

    for bits in &all_bits {
        let x = bits[bit_num] as usize;
        counts[x] += 1;
    }

    if counts.iter().any(|x| *x == 0) {
        return all_bits;
    }
    if counts[0] > counts[1] {
        let mut ret_val: Vec<Vec<u8>> = Vec::new();
        for bits in all_bits {
            if bits[bit_num] == 0 {
                if most_or_least {
                    ret_val.push(bits);
                }
            } else {
                assert_eq!(bits[bit_num], 1);
                if !most_or_least {
                    ret_val.push(bits);
                }
            }
        }
        return ret_val;
    }

    let mut ret_val: Vec<Vec<u8>> = Vec::new();
    for bits in all_bits {
        if bits[bit_num] == 1 {
            if most_or_least {
                ret_val.push(bits);
            }
        } else {
            assert_eq!(bits[bit_num], 0);
            if !most_or_least {
                ret_val.push(bits);
            }
        }
    }
    ret_val
}

fn part_b(all_bits: &Vec<Vec<u8>>) -> u64 {
    let mut prod = 1;
    for i in 0..2 {
        let mut el = all_bits.clone();
        let mut bit_num = 0;
        while el.len() != 1 {
            el = filter_by_bit(el, bit_num, i == 0);
            bit_num += 1;
        }
        prod *= bin_to_dec(&el[0]);
    }
    prod
}

fn main() {
    let bits: Vec<Vec<u8>> = read_lines("./day03.txt")
        .unwrap()
        .map(|l| l.unwrap())
        .map(|l| {
            l.chars()
                .map(|ch| {
                    if ch == '0' {
                        0_u8
                    } else {
                        assert_eq!(ch, '1');
                        1_u8
                    }
                })
                .collect()
        })
        .collect();
    println!("Part A: {}", part_a(&bits));
    println!("Part B: {}", part_b(&bits));
}
