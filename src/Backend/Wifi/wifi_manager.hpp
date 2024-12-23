#ifndef WIFI_MANAGER_HPP
#define WIFI_MANAGER_HPP

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