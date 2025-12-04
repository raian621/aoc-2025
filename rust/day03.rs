use std::{
    cmp::max,
    env,
    io::{prelude::*, stdin, BufReader},
    process::exit,
};

fn main() {
    let solution_part = get_solution_part_arg();
    let banks = parse_banks(&mut BufReader::new(stdin()));
    println!(
        "{}",
        match solution_part.as_str() {
            "1" => solution1(&banks),
            "2" => solution2(&banks),
            _ => panic!("unreachable"),
        }
    )
}

fn get_solution_part_arg() -> String {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 || args[1] != "1" && args[1] != "2" {
        println!("Usage: {} <solution-part>", args[0]);
        exit(1);
    }
    args[1].clone()
}

fn parse_banks<R: BufRead>(buf: &mut R) -> Vec<Vec<u64>> {
    buf.lines()
        .filter_map(|line| line.ok())
        .map(|line| line.chars().map(|c| c as u64 - '0' as u64).collect())
        .collect()
}

fn solution1(banks: &Vec<Vec<u64>>) -> u64 {
    banks
        .iter()
        .map(|bank| get_max_joltage(compute_max_prefix_and_postfix(bank)))
        .reduce(|sum, joltage| sum + joltage)
        .unwrap_or(0)
}

fn solution2(banks: &Vec<Vec<u64>>) -> u64 {
    banks
        .iter()
        .map(|bank| get_max_joltage_dp(bank))
        .reduce(|sum, joltage| sum + joltage)
        .unwrap_or(0)
}

fn compute_max_prefix_and_postfix(banks: &Vec<u64>) -> (Vec<u64>, Vec<u64>) {
    let mut max_prefix = banks.clone();
    let mut max_postfix = banks.clone();
    for i in 1..max_prefix.len() {
        max_prefix[i] = max(max_prefix[i], max_prefix[i - 1]);
    }
    for i in (0..max_postfix.len() - 1).rev() {
        max_postfix[i] = max(max_postfix[i], max_postfix[i + 1]);
    }
    (max_prefix, max_postfix)
}

fn get_max_joltage((max_prefix, max_postfix): (Vec<u64>, Vec<u64>)) -> u64 {
    (0..max_prefix.len() - 1)
        .map(|i| 10 * max_prefix[i] + max_postfix[i + 1])
        .reduce(|max_joltage, joltage| max(max_joltage, joltage))
        .unwrap_or(0)
}

/*
given bank, a list of joltage values,
i : index in bank
c : how many choices are left
M(i, c) = {
    max(
        10 * M(i - 1, c - 1)) + bank[i] ; take the battery at index i
        M(i - 1, c)                     ; skip the battery at index i
    ) when i + 1 < c
    10 * M(i - 1, c - 1)) + bank[i] otherwise
}
*/

// Could be optimized using just two vectors but whatever
fn get_max_joltage_dp(bank: &Vec<u64>) -> u64 {
    let indices = bank.len();
    let choices = 12;

    let mut dp = vec![vec![0; choices]; indices];
    dp[0][0] = bank[0];
    for i in 1..indices {
        dp[i][0] = max(bank[i], dp[i - 1][0]);
        for c in 1..choices {
            // c is one less than in the equation above
            dp[i][c] = if i < c {
                10 * dp[i - 1][c - 1]
            } else {
                max(10 * dp[i - 1][c - 1] + bank[i], dp[i - 1][c])
            }
        }
    }

    dp[indices - 1][choices - 1]
}
