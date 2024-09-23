#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <LittleFS.h>
#include <Ticker.h>
#include "wifi_driver.hpp"
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <ESPAsyncTCP.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

extern void initServerAndWebPage(void);

unsigned long ota_progress_millis = 0;

void setupArduinoOTA(void)
{

  ArduinoOTA.onStart([]()
                     {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type); });
  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    } });
  ArduinoOTA.begin();
  Serial.println("Ready");
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Setup function
void setup()
{
  // Initialize Serial for debugging
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  LittleFS.begin();
  // Initialize LittleFS
  if (!LittleFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }

  initWifi();
  setupArduinoOTA();

  // Initialize server and WebSocket
  initServerAndWebPage();
}

// Loop function
void loop()
{
  server.handleClient();
  webSocket.loop();
  ArduinoOTA.handle();
}
