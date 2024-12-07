#pragma once
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include "device_manager.hpp"
#include <DNSServer.h>

class WiFiManager
{
private:
    ESP8266WebServer &server;
    DeviceManager *deviceManager;
    DNSServer dnsServer;
    bool connect(const String &ssid, const String &password);
    void setupConfigRoutes();
    void setupAccessPoint();
    void setupOTA();
    void setupDNS();

public:
    WiFiManager(ESP8266WebServer &server, DeviceManager *dm)
        : server(server), deviceManager(dm) {}

    void begin();
    void handleClient();
};