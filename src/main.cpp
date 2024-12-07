// #include <Arduino.h>
// #include <Ticker.h>
// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
// #include <WebSocketsServer.h>
// #include <LittleFS.h>
// #include "FS.h"
// #include <ArduinoJson.h>
// #include <ESP8266mDNS.h>
// #include <WiFiUdp.h>
// #include <ArduinoOTA.h>
// #include "device_manager.hpp"
// #include "wifi_manager.hpp"

// ESP8266WebServer server(80);
// DeviceManager deviceManager;
// WiFiManager wifiManager(server, &deviceManager);

// void setup()
// {
//   Serial.begin(115200);

//   if (!LittleFS.begin())
//   {
//     Serial.println("Failed to mount file system");
//     return;
//   }

//   deviceManager.begin();
//   wifiManager.begin();
// }

// void loop()
// {
//   wifiManager.handleClient();
//   server.handleClient();
// }

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <LittleFS.h>
#include "device_manager.hpp"
#include "wifi_manager.hpp"
#include "OTAManager.hpp"

ESP8266WebServer server(80);
DeviceManager deviceManager;
WiFiManager wifiManager(server, &deviceManager);
OTAManager otaManager(&deviceManager);

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  if (!LittleFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }

  deviceManager.begin();
  wifiManager.begin();
  otaManager.begin();
}

void loop()
{
  wifiManager.handleClient();
  server.handleClient();
  otaManager.handle();
}