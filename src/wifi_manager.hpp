#pragma once
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "device_manager.hpp"

class WiFiManager
{
private:
    ESP8266WebServer &server;
    DeviceManager *deviceManager;

    bool connect(const String &ssid, const String &password);
    void setupConfigRoutes();
    void setupAccessPoint();

public:
    WiFiManager(ESP8266WebServer &server, DeviceManager *dm)
        : server(server), deviceManager(dm) {}

    void begin();
    void handleClient() { server.handleClient(); }
};