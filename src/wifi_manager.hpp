// wifi_manager.hpp
#ifndef WIFI_MANAGER_HPP
#define WIFI_MANAGER_HPP

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "device_manager.hpp"

class WiFiManager
{
private:
    void setupConfigRoutes();
    bool connect(const String &ssid, const String &password);

    ESP8266WebServer &server;
    DeviceManager *deviceManager;

public:
    WiFiManager(ESP8266WebServer &serverRef, DeviceManager *deviceManagerRef)
        : server(serverRef), deviceManager(deviceManagerRef) {}

    void begin();
    void handleClient() { server.handleClient(); }
    void setupAccessPoint();
};

#endif // WIFI_MANAGER_HPP