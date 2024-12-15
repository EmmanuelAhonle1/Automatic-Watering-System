#pragma once
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include "../Device/device_manager.hpp"
#include <DNSServer.h>

class WiFiManager
{
private:
    ESP8266WebServer &server;
    DeviceManager *deviceManager;
    DNSServer dnsServer;

    /**
     * @brief Connect to a WiFi network
     * Attempts to connect to the specified WiFi network.
     * @param ssid The SSID of the WiFi network.
     * @param password The password of the WiFi network.
     * @return true if connected successfully, false otherwise.
     */
    bool connect(const String &ssid, const String &password);
    void setupConfigRoutes();
    void setupAccessPoint();
    void setupOTA();
    void setupDNS();
    void sendCaptivePortalSuccess(const String &contentType, const String &content);
    void disableAccessPoint();

public:
    WiFiManager(ESP8266WebServer &server, DeviceManager *dm)
        : server(server), deviceManager(dm) {}

    /**
     * @brief Initialize the WiFiManager
     * Sets up the access point, DNS server, and configuration routes.
     */
    void begin();

    /**
     * @brief Handle client requests
     * Processes DNS and HTTP requests, and handles OTA updates.
     */
    void handleClient();
};