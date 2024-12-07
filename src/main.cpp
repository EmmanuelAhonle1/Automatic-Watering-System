#include <Arduino.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <LittleFS.h>
#include "FS.h"
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "device_manager.hpp"
#include "wifi_manager.hpp"

ESP8266WebServer server(80);
DeviceManager deviceManager;
WiFiManager wifiManager(server, &deviceManager);

void setup()
{
  Serial.begin(115200);

  if (!LittleFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }

  deviceManager.begin();
  wifiManager.begin();
}

void loop()
{
  wifiManager.handleClient();
  server.handleClient();
}