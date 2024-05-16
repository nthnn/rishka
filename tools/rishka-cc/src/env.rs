/* 
 * This file is part of the Rishka distribution (https://github.com/nthnn/rishka).
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

extern crate colored;

use crate::io;
use colored::Colorize;
use std::env;
use std::process::exit;

pub struct RishkaEnv {
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

pub fn check_req_env() -> RishkaEnv {
    RishkaEnv {
        library: check_env_path("RISHKA_LIBPATH"),
        scripts: check_env_path("RISHKA_SCRIPTS")
    }
}