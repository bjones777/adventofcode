use aoc_2021::*;

struct Board {
    numbers: Vec<i32>,
    was_called: Vec<bool>,
    has_won: bool,
}

const SIZE: usize = 5;

impl Board {
    fn call(&mut self, v: i32) -> i32 {
        let mut was_set = false;

        for i in 0..SIZE {
            for j in 0..SIZE {
                if self.get(i, j) == v {
                    self.was_called[i * SIZE + j] = true;
                    was_set = true;
                }
            }
        }

        if was_set {
            self.calc_bingo(v)
        } else {
            -1
        }
    }

    fn get(&self, row: usize, col: usize) -> i32 {
        return self.numbers[row * SIZE + col];
    }

    fn get_called(&self, row: usize, col: usize) -> bool {
        return self.was_called[row * SIZE + col];
    }

    fn calc_bingo(&mut self, v: i32) -> i32 {
        if self.has_won {
            return -1;
        }
        for r in 0..SIZE {
            let s = self.calc_bring_row(v, r);
            if s != -1 {
                self.has_won = true;
                return s;
            }
        }
        for c in 0..SIZE {
            let s = self.calc_bring_col(v, c);
            if s != -1 {
                self.has_won = true;
                return s;
            }
        }
        -1
    }

    fn calc_bring_row(&self, v: i32, row: usize) -> i32 {
        for col in 0..SIZE {
            if !self.get_called(row, col) {
                return -1;
            }
        }
        self.calc_unmarked_score() * v
    }

    fn calc_bring_col(&self, v: i32, col: usize) -> i32 {
        for row in 0..SIZE {
            if !self.get_called(row, col) {
                return -1;
            }
        }
        self.calc_unmarked_score() * v
    }

    fn calc_unmarked_score(&self) -> i32 {
        let mut sum = 0;
        for r in 0..SIZE {
            for c in 0..SIZE {
                if !self.get_called(r, c) {
                    sum += self.get(r, c);
                }
            }
        }
        sum
    }
}

fn build_board(lines: &[String]) -> Board {
    let mut numbers: Vec<i32> = Vec::with_capacity(SIZE * SIZE);
    let was_called: Vec<bool> = vec![false; SIZE * SIZE];
    for line in lines {
        for num in line
            .split_ascii_whitespace()
            .map(|s| s.parse::<i32>().unwrap())
        {
            numbers.push(num);
        }
    }
    Board {
        numbers,
        was_called,
        has_won: false,
    }
}

fn part_a(calls: &Vec<i32>, mut boards: Vec<Board>) -> i32 {
    for call in calls {
        for board in &mut boards {
            let s = board.call(*call);
            if s != -1 {
                return s;
            }
        }
    }
    -1
}

fn part_b(calls: &Vec<i32>, mut boards: Vec<Board>) -> i32 {
    let mut last_score = -1;
    for call in calls {
        for board in &mut boards {
            let s = board.call(*call);
            if s != -1 {
                last_score = s;
            }
        }
    }
    last_score
}

fn gen_boards(lines: &Vec<String>) -> Vec<Board> {
    let num_boards = (lines.len() - 1) / 6;
    assert!((lines.len() - 1) % 6 == 0);
    let mut boards: Vec<Board> = Vec::with_capacity(num_boards);
    for i in 0..num_boards {
        let start_index = 1 + 6 * i;
        boards.push(build_board(&lines[(start_index + 1)..(start_index + 6)]))
    }
    boards
}

fn main() {
    let lines: Vec<String> = read_lines("./day04.txt")
        .unwrap()
        .map(|l| l.unwrap())
        .collect();

    let calls: Vec<i32> = lines[0]
        .split(",")
        .map(|s| s.parse::<i32>().unwrap())
        .collect();
    println!("Part A: {}", part_a(&calls, gen_boards(&lines)));
    println!("Part B: {}", part_b(&calls, gen_boards(&lines)));
}
