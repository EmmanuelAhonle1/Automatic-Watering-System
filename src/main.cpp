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
#include <ArduinoOTA.h>

const int LED_PIN = LED_BUILTIN; // Built-in LED
int blinkPattern = 1;            // Will be changed in different versions

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin("YourSSID", "YourPassword");

  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Configure OTA
  ArduinoOTA.onStart([]()
                     {
                       Serial.println("Starting OTA update");
                       digitalWrite(LED_PIN, HIGH); // Turn off LED during update
                     });

  ArduinoOTA.onEnd([]()
                   {
                     Serial.println("\nOTA Update Complete!");
                     digitalWrite(LED_PIN, LOW); // Turn on LED when complete
                   });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });

  ArduinoOTA.onError([](ota_error_t error)
                     {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

  ArduinoOTA.begin();

  Serial.println("Ready for OTA");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  ArduinoOTA.handle();

  // Blink pattern 1: Single blink
  if (blinkPattern == 1)
  {
    digitalWrite(LED_PIN, LOW); // LED ON
    delay(200);
    digitalWrite(LED_PIN, HIGH); // LED OFF
    delay(800);
  }

  // After successful OTA update, change this to pattern 2:
  /*
  if (blinkPattern == 2) {
      // Double blink
      digitalWrite(LED_PIN, LOW);   // LED ON
      delay(200);
      digitalWrite(LED_PIN, HIGH);  // LED OFF
      delay(200);
      digitalWrite(LED_PIN, LOW);   // LED ON
      delay(200);
      digitalWrite(LED_PIN, HIGH);  // LED OFF
      delay(400);
  }
  */
}