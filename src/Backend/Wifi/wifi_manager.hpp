#ifndef WIFI_MANAGER_HPP
#define WIFI_MANAGER_HPP

#define DEBUG_WIFI_MANAGER

#define USING_DBCONNECTION
// #define USING_DBQC

// Mutual exclusion check
#if defined(USING_DBCONNECTION) && defined(USING_DBQC)
#error "Cannot define both USING_DBCONNECTION and USING_DBQC. Choose one."
#endif

#if !defined(USING_DBCONNECTION) && !defined(USING_DBQC)
#error "Must define either USING_DBCONNECTION or USING_DBQC"
#endif

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "../Device/device_manager.hpp"

class WiFiManager
{
public:
    WiFiManager(ESP8266WebServer &server, DeviceManager *deviceManager); // Updated constructor
    void begin();
    void handleClient();
    bool connect(const String &ssid, const String &password);
    void reconnectWiFi();
    void disableAccessPoint();
    void pingWiFi(); // Declare pingWiFi method

private:
    void setupAccessPoint();
    void setupDNS();
    void setupConfigRoutes();
    void sendCaptivePortalSuccess(const String &contentType, const String &content);

    DNSServer dnsServer;
    ESP8266WebServer &server;
    DeviceManager *deviceManager; // Updated to pointer
};

#endif // WIFI_MANAGER_HPP