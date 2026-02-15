#include <Arduino.h>
#include <Ticker.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include "FS.h"
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "Device/device_manager.hpp"
#include "Device/Pinouts.hpp"
#include "Wifi/wifi_manager.hpp"
#include "RGBStatus/rgb_status.hpp"

WebServer server(80);
DeviceManager deviceManager;
WiFiManager wifiManager(server, &deviceManager);

RGBStateHandler rgbStateHandler;

Ticker rgbTimer;
Ticker dataAcquisitionTimer;
Ticker pingDatabaseTimer;

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
  rgbTimer.attach_ms(100, []()
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