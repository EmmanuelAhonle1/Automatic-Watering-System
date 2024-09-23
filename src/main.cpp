// #include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
// #include <WebSocketsServer.h>
// #include <LittleFS.h>
// #include <Ticker.h>
// #include "wifi_driver.hpp"
// #include <ESP8266WebServer.h>

// #include <ESPAsyncTCP.h>

// extern void initServerAndWebPage(void);
// extern void initWiFiChecker(void);

// unsigned long ota_progress_millis = 0;

// void onOTAStart()
// {
//   Serial.println("OTA update started!");
// }

// void onOTAProgress(size_t current, size_t final)
// {
//   // Log every 1 second
//   if (millis() - ota_progress_millis > 1000)
//   {
//     ota_progress_millis = millis();
//     Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
//   }
// }

// void onOTAEnd(bool success)
// {
//   // Log when OTA has finished
//   if (success)
//   {
//     Serial.println("OTA update finished successfully!");
//   }
//   else
//   {
//     Serial.println("There was an error during OTA update!");
//   }
// }

// // Setup function
// void setup()
// {
//   // Initialize Serial for debugging
//   Serial.begin(9600);
//   pinMode(LED_BUILTIN, OUTPUT);
//   LittleFS.begin();
//   // Initialize LittleFS
//   if (!LittleFS.begin())
//   {
//     Serial.println("Failed to mount file system");
//     return;
//   }

//   // Connect to Wi-Fi
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting to WiFi");
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }

//   server.send(200, "text/plain", "helloo this is Elegant");

//   // Initialize server and WebSocket
//   initServerAndWebPage();

//   // Initialize WiFi checker
//   initWiFiChecker();
// }

// // Loop function
// void loop()
// {
//   server.handleClient();
//   webSocket.loop();
// }
