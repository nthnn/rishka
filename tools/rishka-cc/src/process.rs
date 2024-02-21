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

extern crate colored;

use crate::args::Options;
use crate::env::RishkaEnv;
use colored::Colorize;
use std::process::Command;
use std::process::exit;

fn test_process(name: &str) -> bool {
    match Command::new(name).arg("--version").output() {
        Ok(proc)=> proc.status.success(),
        Err(_)=> false
    }
}

fn check_dep(dep: &str) {
    print!("Checking {}... ", dep.cyan().italic());
    if test_process(dep) {
        println!("{}.", "installed".green().bold());
    } else {
        println!("{} installed.", "not".red().bold());
        exit(0);
    }
}

pub fn run_riscv64_gpp(options: &Options, cc_env: RishkaEnv) -> bool {
    match Command::new("riscv64-unknown-elf-g++")
        .arg("-march=rv64im")
        .arg("-mabi=lp64")
        .arg("-nostdlib")
        .arg("-O2")
        .arg(format!("-Wl,-T,{}/link.ld", cc_env.scripts))
        .arg(format!("-I{}", cc_env.library))
        .arg(format!("-o{}.out", options.output))
        .arg(format!("{}/librishka_impl.cpp", cc_env.library))
        .arg(format!("{}/launcher.s", cc_env.scripts))
        .arg(options.files.join(" "))
        .output() {
        Ok(proc)=> proc.status.success(),
        Err(_)=> false
    }
}

pub fn run_riscv64_objcopy(options: &Options) -> bool {
    match Command::new("riscv64-unknown-elf-objcopy")
        .arg("-O")
        .arg("binary")
        .arg(format!("{}.out", options.output))
        .arg(format!("{}.bin", options.output))
        .output() {
        Ok(proc)=> proc.status.success(),
        Err(_)=> false
    }
}

pub fn check_req_deps() {
    check_dep("riscv64-unknown-elf-g++");
    check_dep("riscv64-unknown-elf-objcopy");
}