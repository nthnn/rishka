extern crate clap;

use clap::*;

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