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
#include "Backend/Device/device_manager.hpp"
#include "Backend/Wifi/wifi_manager.hpp"
#include "Backend/RGBStatus/rgb_status.hpp"
#include <Ticker.h>

ESP8266WebServer server(80);
DeviceManager deviceManager;
WiFiManager wifiManager(server, &deviceManager);

RGBStateHandler rgbStateHandler;

Ticker timer;

#define LED_BUILTIN D4
bool ledState = false;

void setup()
{
  Serial.begin(115200);

  Serial.println();

  if (!LittleFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }
  timer.attach_ms(100, []()
                  { rgbStateHandler.updateRGBStateISR(); });

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  deviceManager.begin();
  wifiManager.begin();
}

void loop()
{
  wifiManager.handleClient();
  server.handleClient();
}