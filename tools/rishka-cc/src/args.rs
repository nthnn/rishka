/* 
 * This file is part of the Rishka distribution (https://github.com/rishka-esp32/rishka-sdk).
 * Copyright (c) 2024 Nathanne Isip.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

extern crate clap;

use crate::banner;
use clap::*;
use std::process::exit;

pub struct Options {
    pub flags:  String,
    pub output: String,
    pub files:  Vec<String>
}

fn parse_args() -> ArgMatches {
    Command::new("rishka-cc")
        .disable_help_flag(true)
        .ignore_errors(true)        
        .arg(Arg::new("flags")
            .short('f')
            .long("flags")
            .value_parser(value_parser!(String))
            .action(ArgAction::Set))
        .arg(Arg::new("output")
            .short('o')
            .long("output")
            .value_parser(value_parser!(String))
            .action(ArgAction::Set))
        .arg(Arg::new("file")
            .value_parser(value_parser!(String))
            .action(ArgAction::Append))
        .get_matches()
}

pub fn get_args() -> Options {
    let argv: ArgMatches = parse_args();
    match argv.get_occurrences::<String>("file") {
        Some(_)=> {},
        None=> {
            banner::print_usage();
            exit(0);
        }
    };

    let files_vec: Vec<Vec<&String>> = argv.get_occurrences("file")
        .unwrap()
        .map(Iterator::collect)
        .collect();

    Options {
        flags: match argv.get_one::<String>("flags") {
            Some(value)=> value.to_string(),
            None=> "".to_string()
        },
        output: match argv.get_one::<String>("output") {
            Some(value)=> value.to_string(),
            None=> "".to_string()
        },
        files: files_vec.into_iter()
            .flatten()
            .map(|s| s.to_string())
            .collect()
    }
}