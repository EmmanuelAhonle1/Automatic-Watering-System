#include "device_manager.hpp"
#include <ESP8266WiFi.h>

String DeviceManager::generateDeviceName()
{
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const int charsetLength = strlen(charset);
    String suffix = "";

    for (int i = 0; i < 4; i++)
    {
        int randomIndex = random(0, charsetLength);
        suffix += charset[randomIndex];
    }

    return "PlantNode-" + suffix;
}

String DeviceManager::readNameFromCredentials()
{
    if (!LittleFS.exists("/credentials.json"))
    {
        Serial.println("No credentials file found");
        return "";
    }

    File file = LittleFS.open("/credentials.json", "r");
    if (!file)
    {
        Serial.println("Failed to open credentials file");
        return "";
    }

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error)
    {
        Serial.println("Failed to parse credentials file");
        return "";
    }

    if (doc.containsKey("name"))
    {
        String storedName = doc["name"].as<String>();
        Serial.println("Found stored name: " + storedName);
        return storedName;
    }

    return "";
}

void DeviceManager::begin()
{
    randomSeed(ESP.getChipId());
    String storedName = readNameFromCredentials();
    deviceName = storedName.isEmpty() ? generateDeviceName() : storedName;
    Serial.println("Device Name: " + deviceName);
    setupAccessPoint();
}

void DeviceManager::setupAccessPoint()
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