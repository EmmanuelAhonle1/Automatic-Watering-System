#include "wifi_manager.hpp"

void WiFiManager::begin()
{
    setupConfigRoutes();
    server.begin();
}

void WiFiManager::setupConfigRoutes()
{
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
                    creds["name"] = deviceName;
                    serializeJson(creds, file);
                    file.close();
                }
            } else {
                response["success"] = false;
                response["message"] = "Failed to connect to " + ssid;
            }
        }
        
        String jsonResponse;
        serializeJson(response, server); });

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
        
        serializeJson(doc, server); });

    server.on("/wifi-config", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/wifi_config/index.html", "r");
        if (!file) {
            server.send(404, "text/plain", "WiFi configuration page not found");
            return;
        }
        server.streamFile(file, "text/html");
        file.close(); });

    server.on("/wifi-config/styles.css", HTTP_GET, [this]()
              {
        File file = LittleFS.open("/wifi_config/styles.css", "r");
        if (!file) {
            server.send(404, "text/plain", "CSS file not found");
            return;
        }
        server.streamFile(file, "text/css");
        file.close(); });

    server.on("/wifi-config/script.js", HTTP_GET, [this]()
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

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
        delay(1000);
        attempts++;
    }

    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::setupAccessPoint()
{
    WiFi.mode(WIFI_AP);
    bool apStarted = WiFi.softAP(deviceName.c_str());

    if (apStarted)
    {
        Serial.println("Access Point Started");
        Serial.print("Network Name: ");
        Serial.println(deviceName);
        Serial.print("IP Address: ");
        Serial.println(WiFi.softAPIP());
    }
    else
    {
        Serial.println("Failed to start Access Point");
    }
}