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

#include "librishka.h"
#include "librishka_impl.hpp"

void Sys::delay(u64 ms) {
    rishka_sc_1(RISHKA_SC_SYS_DELAY_MS, (long) ms);
}

u64 Sys::micros() {
    return (u64) rishka_sc_0(RISHKA_SC_SYS_MICROS);
}

u64 Sys::millis() {
    return (u64) rishka_sc_0(RISHKA_SC_SYS_MILLIS);
}

i64 Sys::shellexec(string program, i32 argc, string* argv) {
    return (i64) rishka_sc_3(RISHKA_SC_SYS_SHELLEXEC, (i64) program, (i64) argc, (i64) argv);
}

void Sys::exit(i32 code) {
    rishka_sc_1(RISHKA_SC_SYS_EXIT, (i64) code);
}

string Sys::info_str(sysinfos_t key) {
    u32 len = rishka_sc_1(RISHKA_SC_SYS_INFOS, (i64) key);
    return get_rt_string(len);
}

i64 Sys::info_num(sysinfon_t key) {
    return (i64) rishka_sc_1(RISHKA_SC_SYS_INFON, (i64) key);
}

i64 Sys::random() {
    return (i64) rishka_sc_0(RISHKA_SC_SYS_RANDOM);
}

bool Sys::change_dir(char* directory) {
    return (bool) rishka_sc_1(RISHKA_SC_SYS_CD, (i64) directory);
}

string Sys::working_dir() {
    u32 len = rishka_sc_0(RISHKA_SC_SYS_WD);
    return get_rt_string(len);
}