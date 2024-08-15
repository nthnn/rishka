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

bool WiFi::connect(string ssid, string passkey, i32 channel, u8* bssid) {
    return (bool) rishka_sc_4(RISHKA_SC_WIFI_CONNECT, (i64) ssid, (i64) passkey, (i64) channel, (i64) bssid);
}

bool WiFi::reconnect() {
    return (bool) rishka_sc_0(RISHKA_SC_WIFI_RECONNECT);
}

bool WiFi::disconnect(bool wifi_off, bool erase_ap) {
    return (bool) rishka_sc_2(RISHKA_SC_WIFI_DISCONNECT, (i64) wifi_off, (i64) erase_ap);
}

bool WiFi::erase_ap() {
    return (bool) rishka_sc_0(RISHKA_SC_WIFI_ERASE_AP);
}

bool WiFi::is_connected() {
    return (bool) rishka_sc_0(RISHKA_SC_WIFI_IS_CONNECTED);
}

bool WiFi::set_autoreconnect(bool auto_recon) {
    return (bool) rishka_sc_1(RISHKA_SC_WIFI_SET_AUTO_RECONNECT, (i64) auto_recon);
}

bool WiFi::is_autoreconnect() {
    return (bool) rishka_sc_0(RISHKA_SC_WIFI_GET_AUTO_RECONNECT);
}

u8 WiFi::wait_for_result(u64 timeout) {
    return (u8) rishka_sc_1(RISHKA_SC_WIFI_WAIT_FOR_RESULT, timeout);
}

void WiFi::set_min_security(u8 auth_mode) {
    rishka_sc_1(RISHKA_SC_WIFI_SET_MINSEC, (i64) auth_mode);
}

void WiFi::set_scan_method(u8 scan_method) {
    rishka_sc_1(RISHKA_SC_WIFI_SET_SCAN_METHOD, (i64) scan_method);
}

void WiFi::set_sort_method(u8 sort_method) {
    rishka_sc_1(RISHKA_SC_WIFI_SET_SORT_METHOD, (i64) sort_method);
}

u8 WiFi::status() {
    return (u8) rishka_sc_0(RISHKA_SC_WIFI_STATUS);
}

string WiFi::ssid() {
    return get_rt_string(rishka_sc_0(RISHKA_SC_WIFI_SSID));
}

string WiFi::psk() {
    return get_rt_string(rishka_sc_0(RISHKA_SC_WIFI_PSK));
}

string WiFi::bssid() {
    return get_rt_string(rishka_sc_0(RISHKA_SC_WIFI_BSSID));
}

i8 WiFi::rssi() {
    return (i8) rishka_sc_0(RISHKA_SC_WIFI_RSSI);
}

bool WiFi::set_local_ip(string ip) {
    return (bool) rishka_sc_1(RISHKA_SC_WIFI_SET_LOCAL_IP, (i64) ip);
}

bool WiFi::set_gateway_ip(string ip) {
    return (bool) rishka_sc_1(RISHKA_SC_WIFI_SET_GATEWAY_IP, (i64) ip);
}