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

bool NVS::commit() {
    return (bool) rishka_sc_0(RISHKA_SC_NVS_COMMIT);
}

bool NVS::erase_all(bool force) {
    return (bool) rishka_sc_1(RISHKA_SC_NVS_ERASE_ALL, (i64) force);
}

bool NVS::erase(string key, bool force) {
    return (bool) rishka_sc_2(RISHKA_SC_NVS_ERASE, (i64) key, (i64) force);
}

bool NVS::set_i8(string key, i8 value, bool force) {
    return (bool) rishka_sc_3(RISHKA_SC_NVS_SET_I8, (i64) key, (i64) value, (i64) force);
}

bool NVS::set_i16(string key, i16 value, bool force) {
    return (bool) rishka_sc_3(RISHKA_SC_NVS_SET_I16, (i64) key, (i64) value, (i64) force);
}

bool NVS::set_i32(string key, i32 value, bool force) {
    return (bool) rishka_sc_3(RISHKA_SC_NVS_SET_I32, (i64) key, (i64) value, (i64) force);
}

bool NVS::set_i64(string key, i64 value, bool force) {
    return (bool) rishka_sc_3(RISHKA_SC_NVS_SET_I64, (i64) key, (i64) value, (i64) force);
}

bool NVS::set_u8(string key, u8 value, bool force) {
    return (bool) rishka_sc_3(RISHKA_SC_NVS_SET_U8, (i64) key, (i64) value, (i64) force);
}

bool NVS::set_u16(string key, u16 value, bool force) {
    return (bool) rishka_sc_3(RISHKA_SC_NVS_SET_U16, (i64) key, (i64) value, (i64) force);
}

bool NVS::set_u32(string key, u32 value, bool force) {
    return (bool) rishka_sc_3(RISHKA_SC_NVS_SET_U32, (i64) key, (i64) value, (i64) force);
}

bool NVS::set_u64(string key, u64 value, bool force) {
    return (bool) rishka_sc_3(RISHKA_SC_NVS_SET_U64, (i64) key, (i64) value, (i64) force);
}

i8 NVS::get_i8(string key, i8 defvalue) {
    return (i8) rishka_sc_2(RISHKA_SC_NVS_GET_I8, (i64) key, (i64) defvalue);
}

i16 NVS::get_i16(string key, i16 defvalue) {
    return (i8) rishka_sc_2(RISHKA_SC_NVS_GET_I16, (i64) key, (i64) defvalue);
}

i32 NVS::get_i32(string key, i32 defvalue) {
    return (i8) rishka_sc_2(RISHKA_SC_NVS_GET_I32, (i64) key, (i64) defvalue);
}

i64 NVS::get_i64(string key, i64 defvalue) {
    return (i8) rishka_sc_2(RISHKA_SC_NVS_GET_I64, (i64) key, (i64) defvalue);
}

u8 NVS::get_u8(string key, u8 defvalue) {
    return (i8) rishka_sc_2(RISHKA_SC_NVS_GET_U8, (i64) key, (i64) defvalue);
}

u16 NVS::get_u16(string key, u16 defvalue) {
    return (i8) rishka_sc_2(RISHKA_SC_NVS_GET_U16, (i64) key, (i64) defvalue);
}

u32 NVS::get_u32(string key, u32 defvalue) {
    return (i8) rishka_sc_2(RISHKA_SC_NVS_GET_U32, (i64) key, (i64) defvalue);
}

u64 NVS::get_u64(string key, u64 defvalue) {
    return (i8) rishka_sc_2(RISHKA_SC_NVS_GET_U64, (i64) key, (i64) defvalue);
}

bool NVS::set_string(string key, string value, bool force) {
    return (bool) rishka_sc_3(RISHKA_SC_NVS_SET_STRING, (i64) key, (i64) value, (i64) force);
}

string NVS::get_string(string key) {
    return get_rt_string((u32) rishka_sc_1(RISHKA_SC_NVS_GET_STRING, (i64) key));
}

bool NVS::has_wifi_config() {
    return (bool) rishka_sc_0(RISHKA_SC_NVS_HAS_WIFI_CONFIG);
}

bool NVS::set_wifi_ssid(string ssid) {
    return (bool) rishka_sc_1(RISHKA_SC_NVS_SET_WIFI_SSID, (i64) ssid);
}

bool NVS::set_wifi_pword(string pword) {
    return (bool) rishka_sc_1(RISHKA_SC_NVS_SET_WIFI_PWORD, (i64) pword);
}