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

mod args;
mod banner;
mod env;
mod io;
mod process;

use colored::Colorize;
use crate::args::Options;
use crate::env::RishkaEnv;
use std::process::exit;

fn compile_task(argv: Options, envvars: RishkaEnv) {
    print!("{} ELF binary from sources... ", "Building".blue().bold());

    let (compiled, err) = process::run_riscv64_gpp(&argv, envvars);
    if !compiled {
        println!("something went {}.", "wrong".red().bold());
        println!("{}", err);
        exit(0);
    }
    else {
        println!("{}!", "done".yellow().bold());
    }

    print!("{} raw binary output from ELF file... ", "Generating".blue().bold());
    if !process::run_riscv64_objcopy(&argv) {
        println!("something went {}.", "wrong".red().bold());
        exit(0);
    }
    else {
        println!("{}!", "done".yellow().bold());
    }

    print!("{} ELF binary output file... ", "Deleting".red().bold());
    if !io::delete_gpp_output(&argv) {
        println!("something went {}.", "wrong".red().bold());
        exit(0);
    }
    else {
        println!("{}!", "deleted".yellow().bold());
    }
}

fn main() {
    let mut argv: Options = args::get_args();
    process::check_req_deps();

    let envvars: RishkaEnv = env::check_req_env();
    if argv.output == "" {
        argv.output = "a".to_string();
    }

    compile_task(argv, envvars);
    println!("Task {}!", "done".green().bold());
}
