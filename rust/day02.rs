use std::{
    collections::HashSet,
    env,
    io::{prelude::*, read_to_string, stdin, BufReader},
    process::exit,
};

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 || args[1] != "1" && args[1] != "2" {
        println!("Usage: {} <solution-part>", args[0]);
        exit(1);
    }
    let solution_part = &args[1];
    let ranges = &parse_ranges(&mut BufReader::new(stdin()));
    println!(
        "{}",
        match solution_part.as_str() {
            "1" => solution1(ranges),
            "2" => solution2(ranges),
            _ => panic!("this shouldn't be reachable"),
        }
    );
}

fn parse_ranges<R: BufRead>(buf: &mut R) -> Vec<(u64, u64)> {
    let input = read_to_string(buf).unwrap();
    input
        .split(",")
        .map(|range| {
            let range_split = range.split("-").collect::<Vec<&str>>();
            (
                range_split[0].parse::<u64>().unwrap(),
                range_split[1].parse::<u64>().unwrap(),
            )
        })
        .collect::<Vec<(u64, u64)>>()
}

fn solution1(ranges: &Vec<(u64, u64)>) -> u64 {
    ranges
        .iter()
        .flat_map(|(lower, upper)| (*lower..=*upper).filter(|id| invalid_id(*id, 2)))
        .reduce(|sum, count| sum + count)
        .unwrap_or(0)
}

fn solution2(ranges: &Vec<(u64, u64)>) -> u64 {
    ranges
        .iter()
        .flat_map(|(lower, upper)| {
            (*lower..=*upper)
                .flat_map(|id| {
                    // Given the number of digits, d, there can only be 2 to
                    // d repeated patterns in an ID
                    let digits = num_digits(id);
                    (2..=digits).filter_map(move |patterns| {
                        if invalid_id(id, patterns) {
                            Some(id)
                        } else {
                            None
                        }
                    })
                })
                // De-dup IDs; ex. 1111 will be invalid for both 4 patterns and
                // 2 patterns
                .collect::<HashSet<u64>>()
                .into_iter()
        })
        .reduce(|sum, count| sum + count)
        .unwrap_or(0)
}

fn invalid_id(id: u64, patterns: u64) -> bool {
    let digits = num_digits(id);

    // IDs with an odd number of digits are valid
    if digits % patterns != 0 {
        return false;
    }

    let mut id = id;
    let power = 10_u64.pow((digits / patterns) as u32);
    (0..patterns - 1).all(|_| {
        let lo_digits = id % power;
        id /= power;
        let hi_digits = id % power;
        lo_digits == hi_digits
    })
}

fn num_digits(id: u64) -> u64 {
    (id.ilog10() + 1) as u64
}
