#ifndef LIBRISHKA_DRIVERS_WIFI_H
#define LIBRISHKA_DRIVERS_WIFI_H

#include <librishka.h>
#include <librishka/types.h>

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
    WIFI_AUTH_MAX
} WiFiAuthMode;

typedef enum {
    WIFI_FAST_SCAN          = 0x00, /**< Do fast scan, scan will end after find SSID match AP */
    WIFI_ALL_CHANNEL_SCAN   = 0x01  /**< All channel scan, scan will end after scan all the channel */
} WiFiScanMethod;

typedef enum {
    WIFI_CONNECT_AP_BY_SIGNAL   = 0,    /**< Sort match AP in scan list by RSSI */
    WIFI_CONNECT_AP_BY_SECURITY,        /**< Sort match AP in scan list by security mode */
} WiFiSortMethod;

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

#endif