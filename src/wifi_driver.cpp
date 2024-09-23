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
#include "wifi_pass.hpp"
//  WiFi credentials
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

// Create web server object on port 80
ESP8266WebServer server(80);
WebSocketsServer webSocket(81); // Create a WebSocket server object on port 81
int prevWifiStatus;

int statusChecker(void)
{
    int currWiFiStatus = WiFi.status();
    if (currWiFiStatus != prevWifiStatus)
    {
        switch (currWiFiStatus)
        {
        case WL_CONNECTED:
            Serial.println("WiFi connected");
            break;
        case WL_NO_SHIELD:
            Serial.println("No WiFi shield present");
            break;
        case WL_IDLE_STATUS:
            Serial.println("WiFi idle");
            break;
        case WL_CONNECT_FAILED:
            Serial.println("Failed to connect to WiFi");
            break;
        case WL_DISCONNECTED:
            Serial.println("Disconnected from WiFi");
            break;
        default:
            Serial.println("Unknown WiFi status");
            break;
        }
        prevWifiStatus = currWiFiStatus;
    }

    return prevWifiStatus;
}

bool ledOn = false;

// Function to notify WebSocket clients about LED status change
void notifyLEDStatusChange()
{
    String message = ledOn ? "LED is ON" : "LED is OFF";
    webSocket.broadcastTXT(message);
}

void checkWifiStatus(void)
{
    int statusVal = statusChecker();
}

// WebSocket event handler
// WebSocket event handler to print message sent from JavaScript WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    if (type == WStype_TEXT)
    {
        Serial.print("Message from client: ");
        Serial.write(payload, length); // Print the entire message
        Serial.println();              // Add newline for readability
    }
}

void handleRoot()
{
    String path = "/mainPage.html";
    File file = LittleFS.open(path, "r");
    if (!file)
    {
        server.send(404, "text/plain", "File not found 3");
        return;
    }
    server.streamFile(file, "text/html");
    file.close();
}

void handleStyleSheet(void)
{
    String path = "/mainPage.css";
    File file = LittleFS.open(path, "r");
    if (!file)
    {
        server.send(404, "text/plain", ("File not found: %s", path));
    }
    server.streamFile(file, "text/html");
    file.close();
}

void handleScript()
{
    String path = "/mainPage.js";
    File file = LittleFS.open(path, "r");
    if (!file)
    {
        server.send(404, "text/plain", "File not found bruh");
        return;
    }
    server.streamFile(file, "application/javascript");
    file.close();
}

void initServerAndWebPage(void)
{

    WiFi.mode(WIFI_STA);
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Route for root / web page
    server.on("/", HTTP_GET, handleRoot);

    // Route for script
    server.on("/mainPage.js", HTTP_GET, handleScript);

    server.on("/mainPage.css", HTTP_GET, handleStyleSheet);

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Start WebSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
    Serial.println("WebSocket server started");
}

void initWifi(void)
{ // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
}