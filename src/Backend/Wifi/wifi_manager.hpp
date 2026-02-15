#ifndef WIFI_MANAGER_HPP
#define WIFI_MANAGER_HPP

#define DEBUG_WIFI_MANAGER

// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "../Device/device_manager.hpp"

class WiFiManager
{
public:
    /**
     * @brief Construct a new WiFiManager object
     *
     * @param server Reference to the web server
     * @param deviceManager Pointer to the device manager
     */
    WiFiManager(WebServer &server, DeviceManager *deviceManager);

    /**
     * @brief Initialize the WiFi manager
     */
    void begin();

    /**
     * @brief Handle client requests
     */
    void handleClient();

    /**
     * @brief Connect to a WiFi network
     *
     * @param ssid The SSID of the network
     * @param password The password of the network
     * @return true if connected successfully, false otherwise
     */
    bool connect(const String &ssid, const String &password);

    /**
     * @brief
     *
     */
    void reconnectWiFi();

    /**
     * @brief Disable the access point
     */
    void disableAccessPoint();

    /**
     * @brief Ping the WiFi network
     */
    void pingWiFi();

private:
    /**
     * @brief Set up the access point
     *
     * @return true if setup successfully, false otherwise
     */
    bool setupAccessPoint();

    /**
     * @brief Set up the DNS server
     */
    void setupDNS();

    /**
     * @brief Set up configuration routes
     */
    void setupConfigRoutes();

    /**
     * @brief Send a success response for captive portal
     *
     * @param contentType The content type of the response
     * @param content The content of the response
     */
    void sendCaptivePortalSuccess(const String &contentType, const String &content);

    /**
     * @brief Check if the credentials are valid
     *
     * @return true if credentials are valid, false otherwise
     */
    bool checkValidCredentials();

    DNSServer dnsServer;
    WebServer &server;
    DeviceManager *deviceManager;
    bool otaInitialized = false;
};

#endif // WIFI_MANAGER_HPP
