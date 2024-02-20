extern crate colored;

use crate::io;
use colored::Colorize;
use std::env;
use std::process::exit;

pub struct RishkaCc {
    pub library:    String,
    pub scripts:    String
}

pub fn get_env(key: &str) -> String {
    match env::var(key) {
        Ok(val) => val.clone(),
        Err(_) => "".to_string()
    }
}

fn check_env_path(key: &str) -> String {
    print!("Checking '{}'... ", key.cyan().italic());

    let path: String = get_env(key);
    if path.as_str() != "" {
        println!("variable {}.", "found".green().bold());

        if !io::directory_exists(&path) {
            println!("Variable '{}' path does {} exists.",
                key.cyan().italic(),
                "not".red().bold());
            exit(0);
        }

        path
    }
    else {
        println!("variable {} found.", "not".red().bold());
        exit(0);
    }
}

pub fn check_req_env() -> RishkaCc {
    RishkaCc {
        library: check_env_path("RISHKA_LIBPATH"),
        scripts: check_env_path("RISHKA_SCRIPTS")
    }
}