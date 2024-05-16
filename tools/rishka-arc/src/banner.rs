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

use colored::Colorize;
 
fn print_banner() {
    println!("{}", "_______________________________________________".cyan());
    println!("{}", "  ______ _____ _______ _     _ _     _ _______".yellow().bold());
    println!("{}", " |_____/   |   |______ |_____| |____/  |_____|".yellow().bold());
    println!("{}", " |    \\_ __|__ ______| |     | |    \\_ |     |\r\n".yellow().bold());
    println!("{} {}", "      Rishka Application Packager".cyan().bold(), "v0.0.1".italic());
    println!("{}\r\n", "_______________________________________________".cyan());
    println!("Rishka archive tool for packaging application installer.\r\n");
}

fn print_help() {
    println!("{}:", "Usage".underline());
    println!("  {} {} {} {}",
        "rishka-arc".italic().bold(),
        "<binary file>".italic(),
        "<manual file>".italic(),
        "<config file>".italic()
    );
    println!("  {} {}",
        "rishka-arc".italic().bold(),
        "help".italic()
    );

    println!("\r\n{}:", "Arguments".underline()); 
    println!(
        "  {}   Binary file of the application in package.",
        "binary file".italic()
    );
    println!(
        "  {}   Manual files for the application package.",
        "manual file".italic()
    );
    println!(
        "  {}   Manifest configuration file and other\r\n\t\t{}\r\n\t\t{}",
        "config file".italic(),
        "miscellaneous informations about the",
        "application."
    );

    println!("\r\nFor more details see:\r\n  {}",
        "https://github.com/nthnn/rishka".underline()); 
    println!();
}
 
pub fn print_usage() {
    print_banner();
    print_help();
}
