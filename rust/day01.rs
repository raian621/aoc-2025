use std::{
    env,
    io::{prelude::*, stdin, BufReader},
    process::exit,
};

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 || args[1] != "1" && args[1] != "2" {
        println!("Usage: {} <solution-part>", args[0]);
        exit(1);
    }
    let solution_part = &args[1];
    let displacements = &parse_displacements(&mut BufReader::new(stdin()));
    println!(
        "{}",
        match solution_part.as_str() {
            "1" => solution1(displacements),
            "2" => solution2(displacements),
            _ => panic!("this shouldn't be reachable"),
        }
    );
}

fn parse_displacements<R: BufRead>(buf: &mut R) -> Vec<i64> {
    buf.lines()
        .filter_map(|line| line.ok())
        .map(|line| match line.chars().nth(0).unwrap() {
            'L' => -line[1..].parse::<i64>().unwrap(),
            'R' => line[1..].parse::<i64>().unwrap(),
            _ => panic!("this shouldn't be reachable"),
        })
        .collect()
}

fn solution1(displacements: &Vec<i64>) -> usize {
    find_num_zeros(displacements, false)
}

fn solution2(displacements: &Vec<i64>) -> usize {
    find_num_zeros(displacements, true)
}

fn find_num_zeros(displacements: &Vec<i64>, count_passes: bool) -> usize {
    displacements
        .iter()
        .fold((50, 0_usize), |(position, zeros), displacement| {
            if count_passes {
                let dist_from_zero = if *displacement <= 0 {
                    (-position + 100) % 100
                } else {
                    position
                };
                (
                    ((position + displacement) % 100 + 100) % 100,
                    zeros + ((dist_from_zero + displacement.abs()) / 100) as usize,
                )
            } else {
                let position = ((position + displacement) % 100 + 100) % 100;
                (position, zeros + (position % 100 == 0) as usize)
            }
        })
        .1
}
