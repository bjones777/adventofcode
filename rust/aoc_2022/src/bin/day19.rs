use aoc_2022::*;

use regex::Regex;
struct Blueprint {
    id: i32,
    ore_robot_cost_ore: i32,
    clay_robot_cost_ore: i32,
    obisidian_cost_ore: i32,
    obisidian_cost_clay: i32,
    geode_cost_ore: i32,
    geode_cost_obsidian: i32,
}

#[derive(Clone, PartialEq, Eq, Hash)]
struct State {
    ore_robots: i32,
    clay_robots: i32,
    obsidian_robots: i32,
    geode_robots: i32,
    ore: i32,
    clay: i32,
    obsidian: i32,
    geode: i32,
    minutes_left: i32,
    ore_robots_to_build: i32,
    clay_robots_to_build: i32,
    obsidian_robots_to_build: i32,
    geode_robots_to_build: i32,
    can_buy_ore_robots: bool,
    can_buy_clay_robots: bool,
    can_buy_obsidian_robots: bool,
    can_buy_geode_robots: bool,
}

fn could_buy_each_robot(blueprint: &Blueprint,state: &State) -> bool {
    if state.ore < blueprint.geode_cost_ore || state.obsidian < blueprint.geode_cost_obsidian {
        return false;
    }
    if state.ore < blueprint.obisidian_cost_ore || state.clay < blueprint.obisidian_cost_clay {
        return false;
    }
    if state.ore < blueprint.clay_robot_cost_ore {
        return false;
    }
    if state.ore >= blueprint.ore_robot_cost_ore {
        return false;
    }
    true
}

fn get_max_geodes_with_state(blueprint: &Blueprint,state: &State,phase: i32) -> i32 {
    if state.minutes_left == 0 {
        return state.geode
    }
    let ret_val = match phase {
        4 => {
            if state.can_buy_geode_robots {
                let mut new_state = state.clone();
                new_state.can_buy_geode_robots = !(new_state.ore >= blueprint.geode_cost_ore && new_state.obsidian >= blueprint.geode_cost_obsidian);
                let mut max_value = get_max_geodes_with_state( blueprint,&new_state,phase-1);
                while new_state.ore >= blueprint.geode_cost_ore && new_state.obsidian >= blueprint.geode_cost_obsidian {
                    new_state.ore -= blueprint.geode_cost_ore;
                    new_state.obsidian -= blueprint.geode_cost_obsidian;
                    new_state.geode_robots_to_build += 1;
                    new_state.can_buy_geode_robots = !(new_state.ore >= blueprint.geode_cost_ore && new_state.obsidian >= blueprint.geode_cost_obsidian);
                    max_value = i32::max(max_value, get_max_geodes_with_state( blueprint, &new_state, phase-1));
                }
                max_value
            }
            else {
                get_max_geodes_with_state(blueprint, state, phase-1)
            }
        },
        3 => {
            if state.can_buy_obsidian_robots {
                let mut new_state = state.clone();
                new_state.can_buy_obsidian_robots = !(new_state.ore >= blueprint.obisidian_cost_ore && new_state.clay >= blueprint.obisidian_cost_clay);
                let mut max_value = get_max_geodes_with_state( blueprint,&new_state,phase-1);
                while new_state.ore >= blueprint.obisidian_cost_ore && new_state.clay >= blueprint.obisidian_cost_clay {
                    new_state.ore -= blueprint.obisidian_cost_ore;
                    new_state.clay -= blueprint.obisidian_cost_clay;
                    new_state.obsidian_robots_to_build += 1;
                    new_state.can_buy_obsidian_robots = !(new_state.ore >= blueprint.obisidian_cost_ore && new_state.clay >= blueprint.obisidian_cost_clay);
                    max_value = i32::max(max_value, get_max_geodes_with_state( blueprint, &new_state, phase-1));
                }
                max_value
            }
            else {
                get_max_geodes_with_state(blueprint, state, phase-1)
            }
        },
        2 => {
            if state.can_buy_clay_robots {
                let mut new_state = state.clone();
                new_state.can_buy_clay_robots = !(new_state.ore >= blueprint.clay_robot_cost_ore);
                let mut max_value = get_max_geodes_with_state( blueprint,state,phase-1);
                while new_state.ore >= blueprint.clay_robot_cost_ore {
                    new_state.ore -= blueprint.clay_robot_cost_ore;
                    new_state.clay_robots_to_build += 1;
                    new_state.can_buy_clay_robots = !(new_state.ore >= blueprint.clay_robot_cost_ore);
                    max_value = i32::max(max_value, get_max_geodes_with_state( blueprint, &new_state, phase-1));
                }
                max_value
            }
            else {
                get_max_geodes_with_state(blueprint, state, phase-1)
            }
        },
        1 => {
            if state.can_buy_ore_robots {
                let mut new_state = state.clone();
                new_state.can_buy_ore_robots = !(new_state.ore >= blueprint.ore_robot_cost_ore);
                let mut max_value = get_max_geodes_with_state( blueprint,state,phase-1);
                while new_state.ore >= blueprint.ore_robot_cost_ore {
                    new_state.ore -= blueprint.ore_robot_cost_ore;
                    new_state.ore_robots_to_build += 1;
                    new_state.can_buy_ore_robots = !(new_state.ore >= blueprint.ore_robot_cost_ore);
                    max_value = i32::max(max_value, get_max_geodes_with_state( blueprint, &new_state, phase-1));
                }
                max_value
            }
            else {
                get_max_geodes_with_state(blueprint, state, phase-1)
            }
        },
        0 => {
            // if we left money on the table, when we could have spent some, that never makes sense
            if could_buy_each_robot(blueprint,state) {
                return 0;
            }
            let mut new_state = state.clone();
            new_state.ore += new_state.ore_robots;
            new_state.clay += new_state.clay_robots;
            new_state.obsidian += new_state.obsidian_robots;
            new_state.geode += new_state.obsidian_robots;
            new_state.ore_robots += new_state.ore_robots_to_build;
            new_state.clay_robots += new_state.clay_robots_to_build;
            new_state.obsidian_robots += new_state.obsidian_robots_to_build;
            new_state.geode_robots += new_state.geode_robots_to_build;

            if new_state.clay_robots_to_build > 0 || new_state.obsidian_robots_to_build > 0 || new_state.geode_robots_to_build > 0 {
                new_state.can_buy_ore_robots = true;
            }

            if new_state.ore_robots_to_build > 0 || new_state.obsidian_robots_to_build > 0 || new_state.geode_robots_to_build > 0 {
                new_state.can_buy_clay_robots = true;
            }

            if new_state.ore_robots_to_build > 0 || new_state.clay_robots_to_build > 0   || new_state.geode_robots_to_build > 0 {
                new_state.can_buy_obsidian_robots = true;
            }

            if new_state.ore_robots_to_build > 0 || new_state.clay_robots_to_build > 0   || new_state.obsidian_robots_to_build  > 0 {
                new_state.can_buy_geode_robots = true;
            }
            new_state.ore_robots_to_build = 0;
            new_state.clay_robots_to_build = 0;
            new_state.obsidian_robots_to_build = 0;
            new_state.geode_robots_to_build = 0;
            new_state.minutes_left -= 1;
            get_max_geodes_with_state( blueprint, &new_state, 4)
        },
        _ => {
            panic!("Unknown phase {}",phase);
        }
    };
    ret_val
}
 

fn get_max_geodes(blueprint: &Blueprint) -> i32 {
    let initial = State {
        ore_robots: 1,
        clay_robots: 0,
        obsidian_robots: 0,
        geode_robots: 0,
        ore: 0,
        clay: 0,
        obsidian: 0,
        geode: 0,
        minutes_left: 24,
        ore_robots_to_build: 0,
        clay_robots_to_build: 0,
        obsidian_robots_to_build: 0,
        geode_robots_to_build: 0,
        can_buy_ore_robots: true,
        can_buy_clay_robots: true,
        can_buy_obsidian_robots: true,
        can_buy_geode_robots: true,
    };
    get_max_geodes_with_state(&blueprint, &initial, 4)
}


fn part_a(lines: &Vec<Blueprint>) -> i32 {
    let mut quality_level = 0;
    for blueprint in lines {
        let ql = get_max_geodes(blueprint);
        println!("ql {} for blueprint {}", ql, blueprint.id);
        quality_level += ql * blueprint.id;
    }
    quality_level
}

fn part_b() -> i32 {
    0
}

fn main() {
    let line_re =
        Regex::new(r"^Blueprint (\d+). Each ore robot costs (\d+) ore. Each clay robot costs (\d+) ore. Each obsidian robot costs (\d+) ore and (\d+) clay. Each geode robot costs (\d+) ore and (\d+) obsidian.$")
            .unwrap();
    let lines: Vec<Blueprint> = read_lines("./day19.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            if let Some(cap) = line_re.captures(s.as_str()) {
                return Blueprint {
                    id: cap.get(1).unwrap().as_str().parse::<i32>().unwrap(),
                    ore_robot_cost_ore: cap.get(2).unwrap().as_str().parse::<i32>().unwrap(),
                    clay_robot_cost_ore: cap.get(3).unwrap().as_str().parse::<i32>().unwrap(),
                    obisidian_cost_ore: cap.get(4).unwrap().as_str().parse::<i32>().unwrap(),
                    obisidian_cost_clay: cap.get(5).unwrap().as_str().parse::<i32>().unwrap(),
                    geode_cost_ore: cap.get(6).unwrap().as_str().parse::<i32>().unwrap(),
                    geode_cost_obsidian: cap.get(7).unwrap().as_str().parse::<i32>().unwrap(),
                };
            }
            panic!("Couldn't match re {}", s);
        })
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b());
}
