#include "wifi_manager.hpp"
#include <DNSServer.h>
#include "OTAManager.hpp"
#include <Arduino.h>
#include <string>
#include "../Device/device_manager.hpp"
#include "../DB Query Creator/dbQueryCreator.hpp"
#include "../Plant Node/PlantNode.hpp"
#include "../Plant Node/DatabaseCommands.hpp"

using namespace std;
const uint8_t DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);

#define LED_BUILTIN 2

// TODO: Add RGB LED statuses for different states
WiFiManager::WiFiManager(ESP8266WebServer &server, DeviceManager *deviceManager)
    : server(server), deviceManager(deviceManager) {}

// Set up the DNS server for the captive portal
void WiFiManager::setupDNS()
{
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", apIP);
    dnsServer.setTTL(0);

    Serial.println("DNS Server started");
    Serial.println("Captive Portal IP: " + apIP.toString());
}

// Send a success response for the captive portal
void WiFiManager::sendCaptivePortalSuccess(const String &contentType, const String &content)
{
    // Add these headers for better iOS detection
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    server.sendHeader("Clear-Site-Data", "\"*\"");

    // Specific for Apple devices
    if (server.hasHeader("User-Agent") &&
        server.header("User-Agent").indexOf("CaptiveNetworkSupport") >= 0)
    {
        server.sendHeader("X-Apple-Success", "true");
        server.sendHeader("X-Apple-Status", "200"); // Add this line
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
                String ssid = creds["wifi_settings"]["ssid"].as<String>();
                String password = creds["wifi_settings"]["password"].as<String>();

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

    DatabaseCommands::pingDatabase();
    delay(200);

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

    // ----------------- WiFi Configuration -----------------
    // Redirect root URL to /wifi_config/index.html
    server.on("/", HTTP_GET, [this]()
              {
        server.sendHeader("Location", "/wifi_config/index.html", true);
        server.send(302, "text/plain", ""); });

    // Serve the wifi_config HTML file
    server.on("/wifi_config/index.html", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/wifi_config/index.html", "r");
        if (!file) {
            server.send(404, "text/plain", "WiFi configuration page not found");
            return;
        }
        server.streamFile(file, "text/html");
        file.close(); });

    // Serve the wifi_config JavaScript file
    server.on("/wifi_config/script.js", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/wifi_config/script.js", "r");
        if (!file) {
            server.send(404, "text/plain", "JavaScript file not found");
            return;
        }
        server.streamFile(file, "application/javascript");
        file.close(); });

    // Serve the wifi_config CSS file
    server.on("/wifi_config/styles.css", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/wifi_config/styles.css", "r");
        if (!file) {
            server.send(404, "text/plain", "CSS file not found");
            return;
        }
        server.streamFile(file, "text/css");
        file.close(); });

    // ----------------- Intro Login -----------------

    // Serve the intro_login HTML file
    server.on("/intro_login/index.html", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/intro_login/index.html", "r");
        if (!file) {
            server.send(404, "text/plain", "Intro login page not found");
            return;
        }
        server.streamFile(file, "text/html");
        file.close(); });

    // Serve the intro_login JavaScript file
    server.on("/intro_login/script.js", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/intro_login/script.js", "r");
        if (!file) {
            server.send(404, "text/plain", "JavaScript file not found");
            return;
        }
        server.streamFile(file, "application/javascript");
        file.close(); });

    // Serve the intro_login CSS file
    server.on("/intro_login/styles.css", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/intro_login/styles.css", "r");
        if (!file) {
            server.send(404, "text/plain", "CSS file not found");
            return;
        }
        server.streamFile(file, "text/css");
        file.close(); });

    // ----------------- Plant Node Registration -----------------

    // Serve the plant_node_registration HTML file
    server.on("/plant_registration/index.html", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/plant_registration/index.html", "r");
        if (!file) {
            server.send(404, "text/plain", "Plant node registration page not found");
            return;
        }
        server.streamFile(file, "text/html");
        file.close(); });

    // Serve the plant_node_registration JavaScript file
    server.on("/plant_registration/script.js", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/plant_registration/script.js", "r");
        if (!file) {
            server.send(404, "text/plain", "JavaScript file not found");
            return;
        }
        server.streamFile(file, "application/javascript");
        file.close(); });

    // Serve the plant_node_registration CSS file
    server.on("/plant_registration/styles.css", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/plant_registration/styles.css", "r");
        if (!file) {
            server.send(404, "text/plain", "CSS file not found");
            return;
        }
        server.streamFile(file, "text/css");
        file.close(); });

    // ----------------- Captive Portal Handler -----------------
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

        delay(1000); // Add delay to allow frontend time to redirect
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
        
        JsonDocument response;
        
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
                    JsonDocument creds;
                    creds["ssid"] = ssid;
                    creds["password"] = password;
                    creds["nodeName"] = deviceManager->getSSID();
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
        JsonDocument doc;
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

    server.on("/update-credentials", HTTP_POST, [this]()
              {
    if (server.hasArg("plain") == false) {
        server.send(400, "application/json", "{\"error\":\"Body not received\"}");
        return;
    }

    String body = server.arg("plain");
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
        server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
        return;
    }

    String ssid = doc["ssid"];
    String password = doc["password"];
    

    server.send(200, "application/json", "{\"message\":\"Credentials updated\"}"); });

    server.on("/get-MAC", HTTP_GET, [this]()
              {
        String mac = WiFi.macAddress();
        server.send(200, "application/json", "{\"macAddress\":\"" + mac + "\"}"); });

    server.on("/success.html", HTTP_GET, [this]()
              { sendCaptivePortalSuccess("text/html",
                                         "<HTML><HEAD><TITLE>Success</TITLE></HEAD><BODY>Success</BODY></HTML>"); });

    server.on("/library/test/success.html", HTTP_GET, [this]()
              { sendCaptivePortalSuccess("text/html",
                                         "<HTML><HEAD><TITLE>Success</TITLE></HEAD><BODY>Success</BODY></HTML>"); });
}

// Connect to a WiFi network
bool WiFiManager::connect(const String &ssid, const String &password)
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    WiFi.hostname(deviceManager->getSSID());

    int attempts = 0;
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED && attempts < 10)
    {
        Serial.print(".");
        delay(1000);
        attempts++;
    }
    Serial.println();

    // TODO: update this
    if (WiFi.status() == WL_CONNECTED)
    {
        StaticJsonDocument response = DatabaseCommands::getPlantNodeSettings();
        String responseString;
        serializeJson(response, responseString);
        Serial.println(responseString);
    }

    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::reconnectWiFi()
{
    if (LittleFS.exists("/credentials.json"))
    {
        File file = LittleFS.open("/credentials.json", "r");
        if (file)
        {
            JsonDocument creds;
            DeserializationError error = deserializeJson(creds, file);
            file.close();

            if (!error)
            {
                String ssid = creds["wifi_settings"]["ssid"].as<String>();
                String password = creds["wifi_settings"]["password"].as<String>();

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
bool WiFiManager::setupAccessPoint()
{
    // Force disconnect from any existing connection
    WiFi.disconnect();
    delay(100);

    // Set mode explicitly to AP
    WiFi.mode(WIFI_AP);
    delay(100);

    // Configure AP settings
    IPAddress apIP(192, 168, 4, 1);
    IPAddress netMsk(255, 255, 255, 0);

    // Configure network
    WiFi.softAPConfig(apIP, apIP, netMsk);

    // Start AP with generated name
    String apName = deviceManager->getSSID();
    if (apName.isEmpty())
    {
        apName = deviceManager->getDeviceName();
    }

    // Start AP with no password
    bool success = WiFi.softAP(apName.c_str());

    if (success)
    {
        // Setup DNS to capture all requests
        dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
        dnsServer.start(53, "*", apIP);

        Serial.println("AP Started: " + apName);
        Serial.println("AP IP: " + WiFi.softAPIP().toString());
    }
    else
    {
        Serial.println("AP Setup Failed!");
    }

    return success;
}