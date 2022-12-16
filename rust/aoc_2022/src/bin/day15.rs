use aoc_2022::*;

use regex::Regex;

struct SensorData {
    sensor_loc: (i32, i32),
    closest_beacon: (i32, i32),
}

fn manhattan_distance(a: &(i32, i32), b: &(i32, i32)) -> i32 {
    (a.0 - b.0).abs() + (a.1 - b.1).abs()
}

fn part_a(lines: &Vec<SensorData>) -> usize {
    let mut min_x = i32::MAX;
    for sensor in lines {
        let m_dist = manhattan_distance(&sensor.sensor_loc, &sensor.closest_beacon);
        min_x = i32::min(min_x, sensor.sensor_loc.0 - m_dist);
    }
    let mut max_x = i32::MIN;
    for sensor in lines {
        let m_dist = manhattan_distance(&sensor.sensor_loc, &sensor.closest_beacon);
        max_x = i32::max(max_x, sensor.sensor_loc.0 + m_dist);
    }

    let y = 2000000;
    let mut count = 0;
    for x in min_x..=max_x {
        for sensor in lines {
            let to_beacon = manhattan_distance(&sensor.sensor_loc, &sensor.closest_beacon);
            let to_loc = manhattan_distance(&sensor.sensor_loc, &(x, y));
            if to_loc <= to_beacon && (x, y) != sensor.closest_beacon {
                count += 1;
                break;
            }
        }
    }
    count
}

enum CouldBeacon {
    Yes,
    No((i32, i32)),
}

fn could_be_unknown_beacon(lines: &Vec<SensorData>, x: (i32, i32)) -> CouldBeacon {
    for sensor in lines {
        let to_beacon = manhattan_distance(&sensor.sensor_loc, &sensor.closest_beacon);
        if x == sensor.sensor_loc {
            return CouldBeacon::No((x.0 + to_beacon + 1, x.1));
        }
        if x == sensor.closest_beacon {
            return CouldBeacon::No((x.0 + 1, x.1));
        }

        let to_loc = manhattan_distance(&sensor.sensor_loc, &x);
        if to_loc <= to_beacon {
            let down_dist = manhattan_distance(&sensor.sensor_loc, &(sensor.sensor_loc.0, x.1));
            return CouldBeacon::No((sensor.sensor_loc.0 + (to_beacon - down_dist) + 1, x.1));
        }
    }
    CouldBeacon::Yes
}

fn part_b(lines: &Vec<SensorData>) -> i64 {
    let mut x = 0;
    let mut y = 0;
    loop {
        match could_be_unknown_beacon(lines, (x, y)) {
            CouldBeacon::Yes => {
                let mut res = x as i64;
                res *= 4000000i64;
                res += y as i64;
                return res;
            }
            CouldBeacon::No(np) => {
                x = np.0;
                if x > 4000000 {
                    x = 0;
                    y += 1;
                    if y > 4000000 {
                        panic!("Couldn't find beacon!")
                    }
                }
            }
        }
    }
}

fn main() {
    let line_re =
        Regex::new(r"^Sensor at x=(-?\d+), y=(-?\d+). closest beacon is at x=(-?\d+), y=(-?\d+)$")
            .unwrap();
    let lines: Vec<SensorData> = read_lines("./day15.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            if let Some(cap) = line_re.captures(s.as_str()) {
                return SensorData {
                    sensor_loc: (
                        cap.get(1).unwrap().as_str().parse::<i32>().unwrap(),
                        cap.get(2).unwrap().as_str().parse::<i32>().unwrap(),
                    ),
                    closest_beacon: (
                        cap.get(3).unwrap().as_str().parse::<i32>().unwrap(),
                        cap.get(4).unwrap().as_str().parse::<i32>().unwrap(),
                    ),
                };
            }
            panic!("Couldn't match re {}", s);
        })
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
