#include "wifi_manager.hpp"
#include <DNSServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);

void WiFiManager::setupDNS()
{
    // Remove any filtering and respond with our IP to all DNS queries
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", apIP);

    // Debug output
    Serial.println("DNS Server started");
    Serial.println("Captive Portal IP: " + apIP.toString());
}

void WiFiManager::setupOTA()
{
    ArduinoOTA.onStart([]()
                       {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else {  // U_FS
            type = "filesystem";
            LittleFS.end();
        }
        Serial.println("Start updating " + type); });

    ArduinoOTA.onEnd([]()
                     { Serial.println("\nEnd"); });

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
    Serial.println("OTA Ready");
}

void WiFiManager::begin()
{
    setupAccessPoint();
    setupDNS();
    setupConfigRoutes();
    //setupOTA();
    server.begin();
}

void WiFiManager::handleClient()
{
    dnsServer.processNextRequest();
    server.handleClient();
    ArduinoOTA.handle();
}

void WiFiManager::setupConfigRoutes()
{
    // Catch-all handler for captive portal
    server.onNotFound([this]()
                      {
        String host = server.hostHeader();
        if (host.length() == 0) {
            server.send(302, "text/plain", "");
            return;
        }
        
        // Redirect all requests to the configuration page
        server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
        server.send(302, "text/plain", ""); });

    // Add this to your existing setupConfigRoutes()
    server.on("/connecttest.txt", HTTP_GET, [this]()
              { server.send(200, "text/plain", "Microsoft NCSI"); });

    server.on("/ncsi.txt", HTTP_GET, [this]()
              { server.send(200, "text/plain", "Microsoft NCSI"); });

    // Android captive portal detection
    server.on("/generate_204", HTTP_GET, [this]()
              {
        server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
        server.send(302, "text/plain", ""); });

    // Microsoft Windows captive portal detection
    server.on("/ncsi.txt", HTTP_GET, [this]()
              {
        server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
        server.send(302, "text/plain", ""); });

    server.on("/connecttest.txt", HTTP_GET, [this]()
              {
        server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
        server.send(302, "text/plain", ""); });

    // Microsoft Edge and IE
    server.on("/redirect", HTTP_GET, [this]()
              {
        server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
        server.send(302, "text/plain", ""); });

    // Apple Captive Portal detection
    server.on("/hotspot-detect.html", HTTP_GET, [this]()
              {
        server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
        server.send(302, "text/plain", ""); });

    server.on("/fwlink", HTTP_GET, [this]()
              {
        server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
        server.send(302, "text/plain", ""); });

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

    server.on("/", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/wifi_config/index.html", "r");
        if (!file) {
            server.send(404, "text/plain", "WiFi configuration page not found");
            return;
        }
        server.streamFile(file, "text/html");
        file.close(); });

    server.on("/styles.css", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/wifi_config/styles.css", "r");
        if (!file) {
            server.send(404, "text/plain", "CSS file not found");
            return;
        }
        server.streamFile(file, "text/css");
        file.close(); });

    server.on("/script.js", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/wifi_config/script.js", "r");
        if (!file) {
            server.send(404, "text/plain", "JavaScript file not found");
            return;
        }
        server.streamFile(file, "application/javascript");
        file.close(); });
}

bool WiFiManager::connect(const String &ssid, const String &password)
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

    // Set the hostname to the device's name
    WiFi.hostname(deviceManager->getName()); // Add this line

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10)
    {
        delay(1000);
        attempts++;
    }

    setupOTA();
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::setupAccessPoint()
{
    WiFi.mode(WIFI_AP);
    WiFi.hostname(deviceManager->getName()); // Add this line
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