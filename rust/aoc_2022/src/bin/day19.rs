use aoc_2022::*;

use regex::Regex;
use std::collections::HashMap;
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
    max_so_far: i32,
}

fn is_no_purchase_reasonable(blueprint: &Blueprint, state: &State) -> bool {
    if state.ore < blueprint.geode_cost_ore
        || (state.obsidian < blueprint.geode_cost_obsidian && state.obsidian_robots != 0)
    {
        return true;
    }
    if state.ore < blueprint.obisidian_cost_ore
        || (state.clay < blueprint.obisidian_cost_clay && state.clay_robots != 0)
    {
        return true;
    }
    if state.ore < blueprint.clay_robot_cost_ore {
        return true;
    }
    if state.ore < blueprint.ore_robot_cost_ore {
        return true;
    }
    false
}

fn made_bad_choice(blueprint: &Blueprint, state: &State) -> bool {
    if state.ore_robots_to_build != 0 {
        let mut max_ore = blueprint.ore_robot_cost_ore;
        max_ore = i32::max(blueprint.clay_robot_cost_ore, max_ore);
        max_ore = i32::max(blueprint.obisidian_cost_ore, max_ore);
        max_ore = i32::max(blueprint.geode_cost_ore, max_ore);
        if state.ore_robots >= max_ore {
            return true;
        }
    }
    if state.clay_robots_to_build != 0 {
        if state.clay_robots >= blueprint.obisidian_cost_clay {
            return true;
        }
    }
    if state.obsidian_robots_to_build != 0 {
        if state.obsidian_robots >= blueprint.geode_cost_obsidian {
            return true;
        }
    }
    false
}

fn get_max_geodes_with_state(
    memo: &mut HashMap<State, i32>,
    blueprint: &Blueprint,
    state: &mut State,
    phase: i32,
) -> i32 {
    if phase == 4
        && state.max_so_far - state.geode
            >= (state.minutes_left * (state.minutes_left + 1)) / 2
                + state.geode_robots * state.minutes_left
    {
        return 0;
    }
    if state.minutes_left == 0 {
        state.max_so_far = i32::max(state.geode, state.max_so_far);
        return state.geode;
    }
    return match phase {
        4 => {
            let mut max_value = 0;
            if state.ore >= blueprint.geode_cost_ore
                && state.obsidian >= blueprint.geode_cost_obsidian
            {
                state.ore -= blueprint.geode_cost_ore;
                state.obsidian -= blueprint.geode_cost_obsidian;
                state.geode_robots_to_build += 1;

                max_value = i32::max(
                    max_value,
                    get_max_geodes_with_state(memo, blueprint, state, 0),
                );

                state.ore += blueprint.geode_cost_ore;
                state.obsidian += blueprint.geode_cost_obsidian;
                state.geode_robots_to_build -= 1;
            }

            max_value = i32::max(
                max_value,
                get_max_geodes_with_state(memo, blueprint, state, phase - 1),
            );
            max_value
        }
        3 => {
            let mut max_value = 0;
            if state.ore >= blueprint.obisidian_cost_ore
                && state.clay >= blueprint.obisidian_cost_clay
            {
                state.ore -= blueprint.obisidian_cost_ore;
                state.clay -= blueprint.obisidian_cost_clay;
                state.obsidian_robots_to_build += 1;

                max_value = i32::max(
                    max_value,
                    get_max_geodes_with_state(memo, blueprint, state, 0),
                );

                state.ore += blueprint.obisidian_cost_ore;
                state.clay += blueprint.obisidian_cost_clay;
                state.obsidian_robots_to_build -= 1;
            }
            max_value = i32::max(
                max_value,
                get_max_geodes_with_state(memo, blueprint, state, phase - 1),
            );
            max_value
        }
        2 => {
            let mut max_value = 0;
            if state.ore >= blueprint.clay_robot_cost_ore {
                state.ore -= blueprint.clay_robot_cost_ore;
                state.clay_robots_to_build += 1;

                max_value = i32::max(
                    max_value,
                    get_max_geodes_with_state(memo, blueprint, state, 0),
                );

                state.ore += blueprint.clay_robot_cost_ore;
                state.clay_robots_to_build -= 1;
            }

            max_value = i32::max(
                max_value,
                get_max_geodes_with_state(memo, blueprint, state, phase - 1),
            );
            max_value
        }
        1 => {
            let mut max_value = 0;
            if state.ore >= blueprint.ore_robot_cost_ore {
                state.ore -= blueprint.ore_robot_cost_ore;
                state.ore_robots_to_build += 1;

                max_value = i32::max(
                    max_value,
                    get_max_geodes_with_state(memo, blueprint, state, 0),
                );

                state.ore += blueprint.ore_robot_cost_ore;
                state.ore_robots_to_build -= 1;
            }

            max_value = i32::max(
                max_value,
                get_max_geodes_with_state(memo, blueprint, state, phase - 1),
            );
            max_value
        }
        0 => {
            // if we left money on the table, when we could have spent some, that never makes sense
            if state.ore_robots_to_build == 0
                && state.clay_robots_to_build == 0
                && state.obsidian_robots_to_build == 0
                && state.geode_robots_to_build == 0
                && !is_no_purchase_reasonable(blueprint, state)
            {
                return 0;
            }
            if made_bad_choice(blueprint, state) {
                return 0;
            }
            let mut new_state = state.clone();
            new_state.ore += new_state.ore_robots;
            new_state.clay += new_state.clay_robots;
            new_state.obsidian += new_state.obsidian_robots;
            new_state.geode += new_state.geode_robots;
            new_state.ore_robots += new_state.ore_robots_to_build;
            new_state.clay_robots += new_state.clay_robots_to_build;
            new_state.obsidian_robots += new_state.obsidian_robots_to_build;
            new_state.geode_robots += new_state.geode_robots_to_build;
            new_state.ore_robots_to_build = 0;
            new_state.clay_robots_to_build = 0;
            new_state.obsidian_robots_to_build = 0;
            new_state.geode_robots_to_build = 0;
            new_state.minutes_left -= 1;
            let ret_val = get_max_geodes_with_state(memo, blueprint, &mut new_state, 4);
            state.max_so_far = new_state.max_so_far;
            ret_val
        }
        _ => {
            panic!("Unknown phase {}", phase);
        }
    };
}

fn get_max_geodes(blueprint: &Blueprint, num_minutes: i32) -> i32 {
    let mut initial = State {
        ore_robots: 1,
        clay_robots: 0,
        obsidian_robots: 0,
        geode_robots: 0,
        ore: 0,
        clay: 0,
        obsidian: 0,
        geode: 0,
        minutes_left: num_minutes,
        ore_robots_to_build: 0,
        clay_robots_to_build: 0,
        obsidian_robots_to_build: 0,
        geode_robots_to_build: 0,
        max_so_far: 0,
    };
    let mut memo: HashMap<State, i32> = HashMap::new();
    get_max_geodes_with_state(&mut memo, &blueprint, &mut initial, 4)
}

fn part_a(lines: &Vec<Blueprint>) -> i32 {
    let mut quality_level = 0;
    for blueprint in lines {
        let ql = get_max_geodes(blueprint, 24);
        println!("ql {} for blueprint {}", ql, blueprint.id);
        quality_level += ql * blueprint.id;
    }
    quality_level
}

fn part_b(lines: &Vec<Blueprint>) -> i32 {
    let mut prod = 1;
    for blueprint in lines.iter().take(3) {
        let ql = get_max_geodes(blueprint, 32);
        println!("ql {} for blueprint {}", ql, blueprint.id);
        prod *= ql;
    }
    prod
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
    println!("Part B: {}", part_b(&lines));
}
