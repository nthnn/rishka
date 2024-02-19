/* 
 * This file is part of the Rishka distribution (https://github.com/rishka-esp32/rishka).
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

#include <librishka.h>

int main() {
    IO::prints(F("Chip Model: "));
    IO::prints(Sys::info_str(SYSINFO_CHIPMODEL));
    IO::prints(F("\n"));

    IO::prints(F("SDK Version: "));
    IO::prints(Sys::info_str(SYSINFO_SDK_VERSION));
    IO::prints(F("\n"));

    IO::prints(F("Sketch MD5: "));
    IO::prints(Sys::info_str(SYSINFO_SKETCH_MD5));
    IO::prints(F("\n"));

    IO::prints(F("PSRAM size: "));
    IO::printn(Sys::info_num(SYSINFO_PSRAM_SIZE));
    IO::prints(F("\n"));

    IO::prints(F("CPU Frequency: "));
    IO::printn(Sys::info_num(SYSINFO_CPU_FREQ));
    IO::prints(F(" Mhz\n"));

    return 0;
}