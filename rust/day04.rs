use std::{
    collections::HashSet,
    env,
    io::{prelude::*, stdin, BufReader},
    process::exit,
};

#[derive(Debug, PartialEq)]
enum NodeState {
    Roll,
    Empty,
}

const DIRECTIONS: [(isize, isize); 8] = [
    (-1, -1),
    (-1, 0),
    (-1, 1),
    (0, 1),
    (1, 1),
    (1, 0),
    (1, -1),
    (0, -1),
];

fn main() {
    let solution_part = get_solution_part_arg();
    let grid = parse_grid(&mut BufReader::new(stdin()));
    println!(
        "{}",
        match solution_part.as_str() {
            "1" => solution1(&grid),
            "2" => solution2(&grid),
            _ => panic!("unreachable"),
        }
    );
}

fn solution1(grid: &Vec<Vec<NodeState>>) -> usize {
    let roll_positions = grid
        .iter()
        .enumerate()
        .flat_map(|(row_idx, row)| {
            row.iter().enumerate().filter_map(move |(col_idx, node)| {
                if *node == NodeState::Roll {
                    Some((row_idx, col_idx))
                } else {
                    None
                }
            })
        })
        .collect::<HashSet<(usize, usize)>>();
    roll_positions
        .iter()
        .filter(|(row, col)| {
            get_surrounding_roll_count(&roll_positions, *row, *col, grid.len(), grid[0].len()) < 4
        })
        .count()
}

fn solution2(grid: &Vec<Vec<NodeState>>) -> usize {
    let mut roll_node_positions = grid
        .iter()
        .enumerate()
        .flat_map(|(row_idx, row)| {
            row.iter().enumerate().filter_map(move |(col_idx, node)| {
                if *node == NodeState::Roll {
                    Some((row_idx, col_idx))
                } else {
                    None
                }
            })
        })
        .collect::<HashSet<(usize, usize)>>();
    let mut removed_roll_count = 0;
    loop {
        let removable_roll_positions =
            get_removable_roll_positions(&roll_node_positions, grid.len(), grid[0].len());
        if removable_roll_positions.len() == 0 {
            break;
        }
        removed_roll_count += removable_roll_positions.len();
        roll_node_positions = roll_node_positions
            .difference(&removable_roll_positions)
            .cloned()
            .collect();
    }
    removed_roll_count
}

fn get_surrounding_roll_count(
    roll_node_positions: &HashSet<(usize, usize)>,
    row: usize,
    col: usize,
    rows: usize,
    cols: usize,
) -> usize {
    DIRECTIONS
        .iter()
        .map(|(d_row, d_col)| (d_row + row as isize, d_col + col as isize))
        .filter_map(|(row, col)| {
            if row >= 0 && row < rows as isize && col >= 0 && col < cols as isize {
                Some((row as usize, col as usize))
            } else {
                None
            }
        })
        .filter(|(row, col)| roll_node_positions.contains(&(*row, *col)))
        .count()
}

fn get_removable_roll_positions(
    roll_node_positions: &HashSet<(usize, usize)>,
    rows: usize,
    cols: usize,
) -> HashSet<(usize, usize)> {
    roll_node_positions
        .iter()
        .filter(|(row, col)| {
            get_surrounding_roll_count(roll_node_positions, *row, *col, rows, cols) < 4
        })
        .cloned()
        .collect()
}

fn get_solution_part_arg() -> String {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 || args[1] != "1" && args[1] != "2" {
        println!("Usage: {} <solution-part>", args[0]);
        exit(1);
    }
    args[1].clone()
}

fn parse_grid<R: BufRead>(buf: &mut R) -> Vec<Vec<NodeState>> {
    buf.lines()
        .filter_map(|line| line.ok())
        .map(|line| {
            line.chars()
                .map(|c| match c {
                    '@' => NodeState::Roll,
                    '.' => NodeState::Empty,
                    _ => panic!("invalid cell state in grid: {}", c),
                })
                .collect::<Vec<NodeState>>()
        })
        .collect::<Vec<Vec<NodeState>>>()
}
