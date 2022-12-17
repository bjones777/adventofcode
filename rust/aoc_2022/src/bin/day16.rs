use aoc_2022::*;

use regex::Regex;
use std::collections::HashMap;
use std::collections::VecDeque;

struct ValveDesc {
    name: String,
    flow_rate: i32,
    tunnels: Vec<String>,
}

struct Entry {
    current_loc: usize,
    time: i32,
    open: Vec<bool>,
    pressure: i32,
}

struct Entry2 {
    current_loc: usize,
    elephant_loc: usize,
    time: i32,
    open: Vec<bool>,
    pressure: i32,
    num_open: i32,
}

fn to_state_string(e: &Entry) -> String {
    let mut ret_val = String::new();
    ret_val.push_str(e.current_loc.to_string().as_str());
    for o in &e.open {
        ret_val.push(if *o { '+' } else { '-' });
    }
    ret_val
}

fn to_state_string2(e: &Entry2) -> String {
    let mut ret_val = String::new();
    if e.current_loc <= e.elephant_loc {
        ret_val.push_str(e.current_loc.to_string().as_str());
        ret_val.push(',');
        ret_val.push_str(e.elephant_loc.to_string().as_str());
    } else {
        ret_val.push_str(e.elephant_loc.to_string().as_str());
        ret_val.push(',');
        ret_val.push_str(e.current_loc.to_string().as_str());
    }
    for o in &e.open {
        ret_val.push(if *o { '+' } else { '-' });
    }
    ret_val
}

fn put_in_q(in_q: &mut HashMap<String,i32>, queue: &mut VecDeque<Entry>, e: Entry) {
    let state_str = to_state_string(&e);
    match in_q.get_mut(&state_str) {
        Some(v) => {
            if e.pressure <= *v {
                return;
            }
            *v = e.pressure;
        }
        None => {
            in_q.insert(state_str, e.pressure);
        }
    }
    queue.push_back(e);
}

fn part_a(lines: &Vec<ValveDesc>) -> i32 {
    let mut name_to_vert: HashMap<String, usize> = HashMap::new();
    let mut adj_list: Vec<Vec<usize>> = Vec::new();
    let mut flow: Vec<i32> = Vec::new();
    for vd in lines {
        let index = match name_to_vert.get(&vd.name) {
            Some(v) => *v,
            None => {
                flow.push(0);
                adj_list.push(Vec::new());
                name_to_vert.insert(vd.name.clone(), adj_list.len() - 1);
                adj_list.len() - 1
            }
        };
        flow[index] = vd.flow_rate;
        for tun in vd.tunnels.iter() {
            let sub_index = match name_to_vert.get(tun) {
                Some(v) => *v,
                None => {
                    flow.push(0);
                    adj_list.push(Vec::new());
                    name_to_vert.insert(tun.clone(), adj_list.len() - 1);
                    adj_list.len() - 1
                }
            };
            adj_list[index].push(sub_index);
        }
    }
    let mut queue: VecDeque<Entry> = VecDeque::new();
    let mut in_q: HashMap<String, i32> = HashMap::new();
    put_in_q(
        &mut in_q,
        &mut queue,
        Entry {
            current_loc: *name_to_vert.get(&"AA".to_string()).unwrap(),
            time: 30,
            open: vec![false; flow.len()],
            pressure: 0,
        },
    );
    let mut max_pressure = 0;
    while !queue.is_empty() {
        let entry = queue.pop_front().unwrap();
        max_pressure = i32::max(max_pressure, entry.pressure);
        if entry.time <= 0 {
            continue;
        }
        if flow[entry.current_loc] > 0 && !entry.open[entry.current_loc] {
            let mut new_open = entry.open.clone();
            new_open[entry.current_loc] = true;

            put_in_q(
                &mut in_q,
                &mut queue,
                Entry {
                    current_loc: entry.current_loc,
                    time: entry.time - 1,
                    open: new_open,
                    pressure: entry.pressure + flow[entry.current_loc] * (entry.time - 1),
                },
            );
        }
        for t in &adj_list[entry.current_loc] {
            put_in_q(
                &mut in_q,
                &mut queue,
                Entry {
                    current_loc: *t,
                    time: entry.time - 1,
                    open: entry.open.clone(),
                    pressure: entry.pressure,
                },
            );
        }
    }
    max_pressure
}

fn get_best_remaining(flow: &Vec<i32>, time: i32, open: &Vec<bool>) -> i32 {
    let mut best_flows: Vec<i32> = Vec::new();
    for x in flow.iter().enumerate() {
        if !open[x.0] && *x.1 > 0 {
            best_flows.push(*x.1);
        }
    }
    best_flows.sort();
    let mut sum = 0;
    for x in best_flows.iter().rev().enumerate() {
        sum += i32::max(0, (time - (1 + (x.0 as i32) / 2)) * x.1);
    }
    sum
}

fn put_in_q2(
    in_q: &mut HashMap<String, i32>,
    queue: &mut VecDeque<Entry2>,
    e: Entry2,
    max_pressure: i32,
    flow: &Vec<i32>,
) {
    if e.pressure + get_best_remaining(flow, e.time, &e.open) <= max_pressure {
        return;
    }
    let state_str = to_state_string2(&e);
    match in_q.get_mut(&state_str) {
        Some(v) => {
            if e.pressure <= *v {
                return;
            }
            *v = e.pressure;
        }
        None => {
            in_q.insert(state_str, e.pressure);
        }
    }
    queue.push_back(e);
}

fn part_b(lines: &Vec<ValveDesc>) -> i32 {
    let mut name_to_vert: HashMap<String, usize> = HashMap::new();
    let mut adj_list: Vec<Vec<usize>> = Vec::new();
    let mut flow: Vec<i32> = Vec::new();
    for vd in lines {
        let index = match name_to_vert.get(&vd.name) {
            Some(v) => *v,
            None => {
                flow.push(0);
                adj_list.push(Vec::new());
                name_to_vert.insert(vd.name.clone(), adj_list.len() - 1);
                adj_list.len() - 1
            }
        };
        flow[index] = vd.flow_rate;
        for tun in vd.tunnels.iter() {
            let sub_index = match name_to_vert.get(tun) {
                Some(v) => *v,
                None => {
                    flow.push(0);
                    adj_list.push(Vec::new());
                    name_to_vert.insert(tun.clone(), adj_list.len() - 1);
                    adj_list.len() - 1
                }
            };
            adj_list[index].push(sub_index);
        }
    }
    let mut queue: VecDeque<Entry2> = VecDeque::new();
    let mut in_q: HashMap<String, i32> = HashMap::new();
    put_in_q2(
        &mut in_q,
        &mut queue,
        Entry2 {
            current_loc: *name_to_vert.get(&"AA".to_string()).unwrap(),
            elephant_loc: *name_to_vert.get(&"AA".to_string()).unwrap(),
            time: 26,
            open: vec![false; flow.len()],
            pressure: 0,
            num_open: 0,
        },
        0,
        &flow,
    );
    let mut max_pressure = 0;
    let num_valves_to_open: i32 = flow.iter().map(|x| if *x > 0 { 1 } else { 0 }).sum();
    while !queue.is_empty() {
        let entry = queue.pop_front().unwrap();
        max_pressure = i32::max(max_pressure, entry.pressure);
        if entry.time <= 0 {
            continue;
        }
        if entry.num_open == num_valves_to_open {
            continue;
        }
        if entry.pressure + get_best_remaining(&flow, entry.time, &entry.open) <= max_pressure {
            continue;
        }
        // oo
        // om
        // mo
        // mm
        if flow[entry.current_loc] > 0 && !entry.open[entry.current_loc] {
            let mut new_open = entry.open.clone();
            new_open[entry.current_loc] = true;
            if flow[entry.elephant_loc] > 0 && !new_open[entry.elephant_loc] {
                new_open[entry.elephant_loc] = true;

                put_in_q2(
                    &mut in_q,
                    &mut queue,
                    Entry2 {
                        current_loc: entry.current_loc,
                        elephant_loc: entry.elephant_loc,
                        time: entry.time - 1,
                        open: new_open.clone(),
                        pressure: entry.pressure
                            + (flow[entry.elephant_loc] + flow[entry.current_loc])
                                * (entry.time - 1),
                        num_open: entry.num_open + 2,
                    },
                    max_pressure,
                    &flow,
                );
            }
            for t in &adj_list[entry.elephant_loc] {
                put_in_q2(
                    &mut in_q,
                    &mut queue,
                    Entry2 {
                        current_loc: entry.current_loc,
                        elephant_loc: *t,
                        time: entry.time - 1,
                        open: new_open.clone(),
                        pressure: entry.pressure + flow[entry.current_loc] * (entry.time - 1),
                        num_open: entry.num_open + 1,
                    },
                    max_pressure,
                    &flow,
                );
            }
        }
        if flow[entry.elephant_loc] > 0 && !entry.open[entry.elephant_loc] {
            let mut new_open = entry.open.clone();
            new_open[entry.elephant_loc] = true;
            for t in &adj_list[entry.current_loc] {
                put_in_q2(
                    &mut in_q,
                    &mut queue,
                    Entry2 {
                        current_loc: *t,
                        elephant_loc: entry.elephant_loc,
                        time: entry.time - 1,
                        open: new_open.clone(),
                        pressure: entry.pressure + flow[entry.elephant_loc] * (entry.time - 1),
                        num_open: entry.num_open + 1,
                    },
                    max_pressure,
                    &flow,
                );
            }
        }
        for t1 in &adj_list[entry.current_loc] {
            for t2 in &adj_list[entry.elephant_loc] {
                put_in_q2(
                    &mut in_q,
                    &mut queue,
                    Entry2 {
                        current_loc: *t1,
                        elephant_loc: *t2,
                        time: entry.time - 1,
                        open: entry.open.clone(),
                        pressure: entry.pressure,
                        num_open: entry.num_open,
                    },
                    max_pressure,
                    &flow,
                );
            }
        }
    }

    max_pressure
}

fn main() {
    //Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
    let line_re =
        Regex::new(r"^Valve (.*) has flow rate=(\d+); tunnels? leads? to valves? (.*)$").unwrap();

    let lines: Vec<ValveDesc> = read_lines("./day16.txt")
        .unwrap()
        .map(|s| s.unwrap())
        .map(|s| {
            if let Some(cap) = line_re.captures(s.as_str()) {
                return ValveDesc {
                    name: String::from(cap.get(1).unwrap().as_str()),
                    flow_rate: cap.get(2).unwrap().as_str().parse::<i32>().unwrap(),
                    tunnels: cap
                        .get(3)
                        .unwrap()
                        .as_str()
                        .split(", ")
                        .map(|p| String::from(p))
                        .collect(),
                };
            }
            panic!("Couldn't match re {}", s);
        })
        .collect();
    println!("Part A: {}", part_a(&lines));
    println!("Part B: {}", part_b(&lines));
}
