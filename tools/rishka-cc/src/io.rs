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

use crate::args::Options;
use std::fs;

pub fn delete_gpp_output(options: &Options) -> bool {
    fs::remove_file(format!("{}.out", options.output)).is_ok()
}

pub fn directory_exists(directory_path: &str) -> bool {
    match fs::metadata(directory_path) {
        Ok(meta)=> meta.is_dir(),
        Err(_)=> false
    }
}