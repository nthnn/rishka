extern crate colored;

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

pub fn check_req_deps() {
    check_dep("riscv64-unknown-elf-g++");
    check_dep("riscv64-unknown-elf-objcopy");
}