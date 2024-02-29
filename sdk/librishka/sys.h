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

#ifndef LIBRISHKA_SYS_H
#define LIBRISHKA_SYS_H

#include <librishka/types.h>

typedef enum {
    SYSINFO_CHIPCORES,
    SYSINFO_CHIP_REV,
    SYSINFO_CPU_FREQ,
    SYSINFO_CYCLE_COUNT,
    SYSINFO_EFUSE_MAC,
    SYSINFO_FLASH_MODE,
    SYSINFO_FLASH_SPEED,
    SYSINFO_FREE_HEAP,
    SYSINFO_FREE_PSRAM,
    SYSINFO_HEAP_SIZE,
    SYSINFO_MAX_ALLOC_HEAP,
    SYSINFO_MIN_FREE_HEAP,
    SYSINFO_MIN_FREE_PSRAM,
    SYSINFO_PSRAM_SIZE,
    SYSINFO_TEMP_VAL,
    SYSINFO_CARD_TYPE,
    SYSINFO_CARD_SIZE,
    SYSINFO_NUM_SECTORS,
    SYSINFO_SECTOR_SIZE,
    SYSINFO_TOTAL_STORAGE,
    SYSINFO_USED_STORAGE
} sysinfon_t;

typedef enum {
    SD_CARD_NONE,
    SD_CARD_MMC,
    SD_CARD_SD,
    SD_CARD_SDHC,
    SD_CARD_UNKNOWN
} sdcard_t;

typedef enum {
    SYSINFO_CHIPMODEL,
    SYSINFO_SDK_VERSION,
    SYSINFO_SKETCH_MD5
} sysinfos_t;

class Sys final {
public:
    static void delay(u64 ms);
    static u64 micros();
    static u64 millis();
    static i32 shellexec(string program, i32 argc, string* argv);
    static void exit(i32 code);
    static string info_str(sysinfos_t key);
    static i64 info_num(sysinfon_t key);
};

#endif