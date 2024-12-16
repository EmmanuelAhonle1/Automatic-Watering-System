#include "wifi_manager.hpp"
#include <DNSServer.h>
#include "OTAManager.hpp"
#include "../Device/dbConnection.hpp"
#include <Arduino.h>
#include <string>
using namespace std;
const uint8_t DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DatabaseConnection awsDB;
#define LED_BUILTIN 2

// Set up the DNS server for the captive portal
void WiFiManager::setupDNS()
{
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", apIP);

    Serial.println("DNS Server started");
    Serial.println("Captive Portal IP: " + apIP.toString());
}

// Send a success response for the captive portal
void WiFiManager::sendCaptivePortalSuccess(const String &contentType, const String &content)
{
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    server.sendHeader("Clear-Site-Data", "\"*\"");

    if (server.hasHeader("User-Agent") && server.header("User-Agent").indexOf("CaptiveNetworkSupport") >= 0)
    {
        server.sendHeader("X-Apple-Success", "true");
    }

    server.send(200, contentType, content);
}

// Disable the access point and switch to station mode
void WiFiManager::disableAccessPoint()
{
    // Ensure WiFi settings persist through disable/enable
    WiFi.persistent(true);

    // Disable AP
    WiFi.softAPdisconnect(true);

    // Switch to station-only mode
    WiFi.mode(WIFI_STA);

    Serial.println("Access Point disabled and station mode configured for stealth");
}

// Initialize the WiFi manager
void WiFiManager::begin()
{
    bool connected = false;
    Serial.println("on now");
    delay(1000);

    // Check for saved WiFi credentials
    if (LittleFS.exists("/credentials.json"))
    {
        File file = LittleFS.open("/credentials.json", "r");
        if (file)
        {
            JsonDocument creds; // Use StaticJsonDocument
            DeserializationError error = deserializeJson(creds, file);
            file.close();

            if (!error)
            {
                String ssid = creds["ssid"].as<String>();
                String password = creds["password"].as<String>();

                connected = connect(ssid, password);
                if (connected)
                {
                    Serial.println("Connected to [" + ssid + "] using saved credentials");
                }
            }
        }
    }

    // If no saved credentials or connection failed, start AP mode
    if (!connected)
    {
        setupAccessPoint();
        setupDNS();
    }

    // Always set up the web server to serve the necessary files
    setupConfigRoutes();
    server.begin();
    Serial.println("Web server started");

    // Always set up OTA
    setupArduinoOTA();
}

// Handle client requests
void WiFiManager::handleClient()
{
    dnsServer.processNextRequest();
    server.handleClient();
    ArduinoOTA.handle();
    awsDB.connect();

    // Check WiFi connection status and attempt to reconnect if disconnected
    bool savedCredentials = false;
    // Check for saved WiFi credentials
    if (LittleFS.exists("/credentials.json"))
    {
        File file = LittleFS.open("/credentials.json", "r");
        if (file)
        {
            JsonDocument creds; // Use StaticJsonDocument
            DeserializationError error = deserializeJson(creds, file);
            file.close();

            if (!error)
            {
                String ssid = creds["ssid"].as<String>();
                String password = creds["password"].as<String>();

                if (ssid.isEmpty() && password.isEmpty())
                {
                    Serial.println("No saved credentials found");
                }
                else
                {
                    Serial1.println("Saved credentials found");
                    savedCredentials = true;
                }
            }
        }
    }

    if (WiFi.status() != WL_CONNECTED && !savedCredentials)
    {
        Serial.println("WiFi disconnected. Attempting to reconnect...");
        reconnectWiFi();
    }
}

// Set up the configuration routes for the web server
void WiFiManager::setupConfigRoutes()
{
    // Serve the main HTML page
    server.on("/", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/wifi_config/index.html", "r");
        if (!file) {
            server.send(404, "text/plain", "WiFi configuration page not found");
            return;
        }
        server.streamFile(file, "text/html");
        file.close(); });

    // Serve the CSS file
    server.on("/styles.css", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/wifi_config/styles.css", "r");
        if (!file) {
            server.send(404, "text/plain", "CSS file not found");
            return;
        }
        server.streamFile(file, "text/css");
        file.close(); });

    // Serve the JavaScript file
    server.on("/script.js", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/wifi_config/script.js", "r");
        if (!file) {
            server.send(404, "text/plain", "JavaScript file not found");
            return;
        }
        server.streamFile(file, "application/javascript");
        file.close(); });

    // Catch-all handler for captive portal
    server.onNotFound([this]()
                      {
        String host = server.hostHeader();
        if (host.length() == 0) {
            server.send(302, "text/plain", "");
            return;
        }
        
        server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
        server.send(302, "text/plain", ""); });

    // Handle disabling the access point
    server.on("/disable-ap", HTTP_POST, [this]()
              {
        if (server.hasHeader("User-Agent")) {
            String userAgent = server.header("User-Agent");
            
            if (userAgent.indexOf("CaptiveNetworkSupport") >= 0) {
                // iOS device
                sendCaptivePortalSuccess("text/html", 
                    "<HTML><HEAD><TITLE>Success</TITLE></HEAD><BODY>Success</BODY></HTML>");
            } else if (userAgent.indexOf("Microsoft") >= 0) {
                // Windows device
                sendCaptivePortalSuccess("text/plain", "Microsoft NCSI");
            } else {
                // Generic success response
                sendCaptivePortalSuccess("text/plain", "Success");
            }
        } else {
            sendCaptivePortalSuccess("text/plain", "Success");
        }

        disableAccessPoint(); });

    // Captive portal detection endpoints
    server.on("/connecttest.txt", HTTP_GET, [this]()
              {
        if (WiFi.status() == WL_CONNECTED) {
            sendCaptivePortalSuccess("text/plain", "Microsoft NCSI");
        } else {
            server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
            server.send(302, "text/plain", "");
        } });

    server.on("/ncsi.txt", HTTP_GET, [this]()
              {
        if (WiFi.status() == WL_CONNECTED) {
            sendCaptivePortalSuccess("text/plain", "Microsoft NCSI");
        } else {
            server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
            server.send(302, "text/plain", "");
        } });

    server.on("/generate_204", HTTP_GET, [this]()
              {
        if (WiFi.status() == WL_CONNECTED) {
            sendCaptivePortalSuccess("text/plain", "");
        } else {
            server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
            server.send(302, "text/plain", "");
        } });

    server.on("/hotspot-detect.html", HTTP_GET, [this]()
              {
        if (WiFi.status() == WL_CONNECTED) {
            sendCaptivePortalSuccess("text/html", 
                "<HTML><HEAD><TITLE>Success</TITLE></HEAD><BODY>Success</BODY></HTML>");
        } else {
            server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
            server.send(302, "text/plain", "");
        } });

    server.on("/fwlink", HTTP_GET, [this]()
              {
        server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
        server.send(302, "text/plain", ""); });

    // Handle WiFi connection requests
    server.on("/wifi-connect", HTTP_POST, [this]()
              {
        String ssid = server.arg("ssid");
        String password = server.arg("password");
        
        DynamicJsonDocument response(256);
        
        if (ssid.length() == 0) {
            response["success"] = false;
            response["message"] = "SSID cannot be empty";
        } else {
            if (connect(ssid, password)) {
                response["success"] = true;
                response["message"] = "Connected to " + ssid;
                response["ip"] = WiFi.localIP().toString();
                
                // Save credentials
                File file = LittleFS.open("/credentials.json", "w");
                if (file) {
                    DynamicJsonDocument creds(512);
                    creds["ssid"] = ssid;
                    creds["password"] = password;
                    creds["name"] = deviceManager->getName();
                    serializeJson(creds, file);
                    file.close();
                }
            } else {
                response["success"] = false;
                response["message"] = "Failed to connect to " + ssid;
            }
        }
        
        String jsonResponse;
        serializeJson(response, jsonResponse);
        server.send(200, "application/json", jsonResponse); });

    // Handle WiFi scan requests
    server.on("/wifi-scan", HTTP_GET, [this]()
              {
        DynamicJsonDocument doc(1024);
        JsonArray array = doc.to<JsonArray>();
        
        int n = WiFi.scanNetworks();
        for (int i = 0; i < n; ++i) {
            JsonObject network = array.createNestedObject();
            network["ssid"] = WiFi.SSID(i);
            network["rssi"] = WiFi.RSSI(i);
            network["encrypted"] = WiFi.encryptionType(i) != ENC_TYPE_NONE;
        }
        
        String jsonResponse;
        serializeJson(doc, jsonResponse);
        server.send(200, "application/json", jsonResponse); });
}

// Connect to a WiFi network
bool WiFiManager::connect(const String &ssid, const String &password)
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    WiFi.hostname(deviceManager->getName());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10)
    {
        Serial.println(".");
        delay(1000);
        attempts++;
    }

    // setupArduinoOTA();
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::reconnectWiFi()
{
    if (LittleFS.exists("/credentials.json"))
    {
        File file = LittleFS.open("/credentials.json", "r");
        if (file)
        {
            StaticJsonDocument<512> creds;
            DeserializationError error = deserializeJson(creds, file);
            file.close();

            if (!error)
            {
                String ssid = creds["ssid"].as<String>();
                String password = creds["password"].as<String>();

                if (connect(ssid, password))
                {
                    Serial.println("Reconnected to WiFi network [" + ssid + "]");
                }
                else
                {
                    Serial.println("Failed to reconnect to WiFi network [" + ssid + "]");
                }
            }
        }
    }
}

// Set up the access point
void WiFiManager::setupAccessPoint()
{
    WiFi.mode(WIFI_AP);
    WiFi.hostname(deviceManager->getName());
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    bool apStarted = WiFi.softAP(deviceManager->getName().c_str());

    if (apStarted)
    {
        Serial.println("Access Point Started");
        Serial.print("Network Name: ");
        Serial.println(deviceManager->getName());
        Serial.print("IP Address: ");
        Serial.println(apIP.toString());
    }
    else
    {
        Serial.println("Failed to start Access Point");
    }
}