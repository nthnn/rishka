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

/**
 * @file librishka/drivers/wifi.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief WiFi driver SDK for Rishka's SDK
 *
 * This header file defines the WiFi class, which provides a set of methods for managing
 * WiFi connections. It supports connecting to access points, configuring connection
 * parameters, and managing WiFi-related information.
 */

#ifndef LIBRISHKA_DRIVERS_WIFI_H
#define LIBRISHKA_DRIVERS_WIFI_H

#include <librishka.h>
#include <librishka/types.h>

/**
 * @enum WiFiAuthMode
 * @brief Enumerates the supported WiFi authentication modes.
 */
typedef enum {
    WIFI_AUTH_OPEN              = 0x00, /**< Open */
    WIFI_AUTH_WEP,                      /**< WEP */
    WIFI_AUTH_WPA_PSK,                  /**< WPA-PSK */
    WIFI_AUTH_WPA2_PSK,                 /**< WPA2-PSK */
    WIFI_AUTH_WPA_WPA2_PSK,             /**< WPA-WPA2-PSK */
    WIFI_AUTH_ENTERPRISE,               /**< WiFi EAP security */
    WIFI_AUTH_WPA2_ENTERPRISE   = 0x05, /**< WiFi EAP security */
    WIFI_AUTH_WPA3_PSK,                 /**< WPA3-PSK */
    WIFI_AUTH_WPA2_WPA3_PSK,            /**< WPA2-WPA3-PSK */
    WIFI_AUTH_WAPI_PSK,                 /**< WAPI-PSK */
    WIFI_AUTH_WPA3_ENT_192,             /**< WPA3-ENT-SUITE-B 192-BIT */
    WIFI_AUTH_MAX                       /**< Maximum number of authentication modes */
} WiFiAuthMode;

/**
 * @enum WiFiScanMethod
 * @brief Defines the methods used for scanning WiFi networks.
 */
typedef enum {
    WIFI_FAST_SCAN          = 0x00, /**< Do fast scan, scan will end after find SSID match AP */
    WIFI_ALL_CHANNEL_SCAN   = 0x01  /**< All channel scan, scan will end after scan all the channel */
} WiFiScanMethod;

/**
 * @enum WiFiSortMethod
 * @brief Defines the methods for sorting access points in the scan list.
 */
typedef enum {
    WIFI_CONNECT_AP_BY_SIGNAL   = 0,    /**< Sort match AP in scan list by RSSI */
    WIFI_CONNECT_AP_BY_SECURITY,        /**< Sort match AP in scan list by security mode */
} WiFiSortMethod;

/**
 * @enum WiFiStatus
 * @brief Enumerates the possible states of the WiFi connection.
 */
typedef enum {
    WIFI_STATUS_NO_SHIELD       = 0xff,
    WIFI_STATUS_STOPPED         = 0xfe,
    WIFI_STATUS_IDLE_STATUS     = 0x00,
    WIFI_STATUS_NO_SSID_AVAIL   = 0x01,
    WIFI_STATUS_SCAN_COMPLETED  = 0x02,
    WIFI_STATUS_CONNECTED       = 0x03,
    WIFI_STATUS_CONNECT_FAILED  = 0x04,
    WIFI_STATUS_CONNECTION_LOST = 0x05,
    WIFI_STATUS_DISCONNECTED    = 0x06
} WiFiStatus;

/**
 * @class WiFi
 * @brief Provides an interface for managing WiFi connections on the Rishka system.
 *
 * The WiFi class offers a set of static methods for connecting to access points, configuring
 * WiFi parameters, managing connection states, and retrieving WiFi-related information.
 */
class WiFi final {
public:
    /**
     * @brief Connects to a WiFi network.
     * @param ssid The SSID of the WiFi network.
     * @param passkey The password of the WiFi network.
     * @param channel The WiFi channel to connect to (optional; defaults to 0 for automatic channel selection).
     * @param bssid The BSSID of the access point (optional; defaults to nullptr).
     * @return true if the connection was successful, false otherwise.
     *
     * This method attempts to connect to the specified WiFi network using the provided SSID and passkey.
     * Optionally, the channel and BSSID can be specified for more precise control over the connection.
     */
    static bool connect(string ssid, string passkey, i32 channel = 0, u8* bssid = nullptr);

    /**
     * @brief Reconnects to the last connected WiFi network.
     * @return true if the reconnection was successful, false otherwise.
     *
     * This method attempts to reconnect to the last successfully connected WiFi network.
     */
    static bool reconnect();

    /**
     * @brief Disconnects from the current WiFi network.
     * @param wifi_off If true, turns off the WiFi module after disconnecting.
     * @param erase_ap If true, erases the saved access point data after disconnecting.
     * @return true if the disconnection was successful, false otherwise.
     *
     * This method disconnects from the current WiFi network. Optionally, the WiFi module can be turned off
     * and the saved access point data can be erased.
     */
    static bool disconnect(bool wifi_off, bool erase_ap);

    /**
     * @brief Erases the saved access point data from memory.
     * @return true if the data was successfully erased, false otherwise.
     *
     * This method erases the saved access point data from the device's memory.
     */
    static bool erase_ap();

    /**
     * @brief Checks if the device is currently connected to a WiFi network.
     * @return true if the device is connected, false otherwise.
     *
     * This method returns the connection status of the WiFi module.
     */
    static bool is_connected();

    /**
     * @brief Sets the auto-reconnect feature.
     * @param auto_recon If true, enables auto-reconnect; otherwise, disables it.
     * @return true if the operation was successful, false otherwise.
     *
     * This method enables or disables the auto-reconnect feature, which automatically attempts to reconnect
     * to the WiFi network after a disconnection.
     */
    static bool set_autoreconnect(bool auto_recon);

    /**
     * @brief Checks if the auto-reconnect feature is enabled.
     * @return true if auto-reconnect is enabled, false otherwise.
     *
     * This method returns the status of the auto-reconnect feature.
     */
    static bool is_autoreconnect();

    /**
     * @brief Waits for a WiFi connection result within the specified timeout period.
     * @param timeout The timeout period in milliseconds.
     * @return The WiFi connection status after the timeout period or upon connection.
     *
     * This method waits for the result of the WiFi connection attempt, blocking the thread for
     * the specified timeout period. It returns the connection status after the wait period.
     */
    static u8 wait_for_result(u64 timeout);

    /**
     * @brief Sets the minimum security level for WiFi connections.
     * @param auth_mode The minimum authentication mode required for connection (e.g., WPA2, WPA3).
     *
     * This method sets the minimum security level (authentication mode) that is required for
     * connecting to WiFi networks.
     */
    static void set_min_security(u8 auth_mode);

    /**
     * @brief Sets the WiFi scanning method.
     * @param scan_method The scan method to use (e.g., fast scan, all-channel scan).
     *
     * This method configures the scanning method used when searching for WiFi networks.
     */
    static void set_scan_method(u8 scan_method);

    /**
     * @brief Sets the method for sorting access points in the scan list.
     * @param sort_method The sort method to use (e.g., by signal strength, by security).
     *
     * This method configures how the WiFi module sorts the access points found during a scan.
     */
    static void set_sort_method(u8 sort_method);

    /**
     * @brief Retrieves the current status of the WiFi module.
     * @return The current WiFi status (e.g., connected, disconnected, scanning).
     *
     * This method returns the current status of the WiFi module.
     */
    static u8 status();

    /**
     * @brief Retrieves the SSID of the current WiFi network.
     * @return The SSID of the connected WiFi network, or an empty string if not connected.
     *
     * This method returns the SSID of the WiFi network to which the device is currently connected.
     */
    static string ssid();

    /**
     * @brief Retrieves the Pre-Shared Key (PSK) of the current WiFi network.
     * @return The PSK of the connected WiFi network, or an empty string if not connected.
     *
     * This method returns the Pre-Shared Key (PSK) of the WiFi network to which the device is connected.
     */
    static string psk();

    /**
     * @brief Retrieves the BSSID of the current WiFi access point.
     * @return The BSSID of the connected WiFi access point, or an empty string if not connected.
     *
     * This method returns the Basic Service Set Identifier (BSSID) of the access point
     * to which the device is currently connected.
     */
    static string bssid();

    /**
     * @brief Retrieves the Received Signal Strength Indicator (RSSI) of the current WiFi connection.
     * @return The RSSI value in dBm, or a negative value if not connected.
     *
     * This method returns the signal strength (RSSI) of the WiFi network to which the device is connected.
     */
    static i8 rssi();

    /**
     * @brief Sets the local IP address for the WiFi connection.
     * @param ip The IP address to assign.
     * @return true if the IP address was successfully set, false otherwise.
     *
     * This method sets the local IP address for the WiFi connection.
     */
    static bool set_local_ip(string ip);

    /**
     * @brief Sets the gateway IP address for the WiFi connection.
     * @param ip The gateway IP address to assign.
     * @return true if the gateway IP address was successfully set, false otherwise.
     *
     * This method sets the gateway IP address for the WiFi connection.
     */
    static bool set_gateway_ip(string ip);
};

#endif