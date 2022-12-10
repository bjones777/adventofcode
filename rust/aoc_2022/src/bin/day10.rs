use aoc_2022::*;

fn part_a(lines: &Vec<(String, i32)>) -> i64 {
    let mut cycle_counter = 1;
    let mut index = 0;
    let mut sum = 0;
    let mut x = 1;
    let mut start_cycle = 20;
    loop {
        if index >= lines.len() {
            index -= lines.len();
        }
        let old_x = x;
        let old_cycle_counter = cycle_counter;
        let cmd = lines[index].clone();
        match cmd.0.as_str() {
            "addx" => {
                cycle_counter += 2;
                x += cmd.1;
            }
            "noop" => {
                cycle_counter += 1;
            }
            _ => panic!("Unknown command"),
        }
        index += 1;
        if old_cycle_counter < start_cycle && cycle_counter >= start_cycle {
            let x_to_use = if cycle_counter == start_cycle {
                x
            } else {
                old_x
            };
            //println!("sum at {} is {}",start_cycle,x_to_use);
            sum += (start_cycle * x_to_use) as i64;
            start_cycle += 40;
            if start_cycle > 220 {
                break;
            }
        }
    }
    sum
}

fn cycle_to_x(cycle_counter: usize) -> i32 {
    let mut v = cycle_counter as i32;
    v -= 1;
    v %= 40;
    v
}

fn draw_pixel_if_lit(cycle_counter: usize, x: i32, pixels: &mut Vec<bool>) {
    if (cycle_to_x(cycle_counter) - x).abs() <= 1 {
        pixels[cycle_counter] = true;
    }
}

fn part_b(lines: &Vec<(String, i32)>) {
    let mut pixels: Vec<bool> = vec![false; 40 * 6 + 1];
    let mut cycle_counter: usize = 1;
    let mut index = 0;
    let mut x = 1;
    loop {
        if index >= lines.len() {
            index -= lines.len();
        }
        let cmd = lines[index].clone();
        match cmd.0.as_str() {
            "addx" => {
                if cycle_counter >= pixels.len() {
                    break;
                }
                draw_pixel_if_lit(cycle_counter, x, &mut pixels);
                cycle_counter += 1;

                if cycle_counter >= pixels.len() {
                    break;
                }
                draw_pixel_if_lit(cycle_counter, x, &mut pixels);
                cycle_counter += 1;
                x += cmd.1;
            }
            "noop" => {
                if cycle_counter >= pixels.len() {
                    break;
                }
                draw_pixel_if_lit(cycle_counter, x, &mut pixels);
                cycle_counter += 1;
            }
            _ => panic!("Unknown command"),
        }
        index += 1;
    }
    for y in 0..6 {
        for x in 0..40 {
            if pixels[y * 40 + x + 1] {
                print!("#");
            } else {
                print!(" ");
            }
        }
        println!("");
    }
}

fn main() {
    let lines: Vec<(String, i32)> = read_lines("./day10.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            let parts: Vec<&str> = s.split_ascii_whitespace().collect();
            if parts[0] == "noop" {
                (String::from(parts[0]), 0)
            } else {
                (String::from(parts[0]), parts[1].parse::<i32>().unwrap())
            }
        })
        .collect();
    let val = part_a(&lines);
    println!("Part A: {}", val);
    part_b(&lines);
}
