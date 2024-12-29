#include "device_manager.hpp"
#include <ESP8266WiFi.h>

String DeviceManager::generateDeviceName()
{
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const int charsetLength = strlen(charset);
    String suffix = "";

    // Combine multiple sources of entropy
    unsigned long seed = ESP.getChipId(); // Unique chip ID
    seed ^= system_get_time();            // Current system time in microseconds
    seed ^= ESP.getCycleCount();          // CPU cycle count
    seed ^= analogRead(A0);               // Random noise from analog pin
    seed ^= millis();                     // Milliseconds since boot

    // Set the random seed
    randomSeed(seed);

    // Generate a truly random suffix
    for (int i = 0; i < 4; i++)
    {
        // Add extra randomization per character
        seed ^= ESP.getCycleCount();
        seed ^= system_get_time();
        random(charsetLength); // Throw away first value

        int randomIndex = random(charsetLength);
        suffix += charset[randomIndex];
    }

    return "PlantNode-" + suffix;
}

bool DeviceManager::updateStoredName(const String &newName)
{
    if (!LittleFS.exists("/credentials.json"))
    {
        // If file doesn't exist, create new one with just the name
        DynamicJsonDocument doc(512);
        doc["nodeName"] = newName;

        File file = LittleFS.open("/credentials.json", "w");
        if (!file)
        {
            Serial.println("Failed to create credentials file");
            return false;
        }

        serializeJson(doc, file);
        file.close();
        return true;
    }

    // Read existing file
    File file = LittleFS.open("/credentials.json", "r");
    if (!file)
    {
        Serial.println("Failed to open credentials file");
        return false;
    }

    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error)
    {
        Serial.println("Failed to parse credentials file");
        return false;
    }

    // Update name in JSON while preserving other fields
    doc["nodeName"] = newName;

    // Write updated JSON back to file
    file = LittleFS.open("/credentials.json", "w");
    if (!file)
    {
        Serial.println("Failed to open credentials file for writing");
        return false;
    }

    serializeJson(doc, file);
    file.close();

    Serial.println("Device name updated in credentials: " + newName);
    return true;
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

    if (doc.containsKey("nodeName"))
    {
        String storedName;

        if (doc["nodeName"].as<String>().isEmpty())
        {

            Serial.println("No stored name found");
        }
        else
        {
            storedName = doc["nodeName"].as<String>();
            Serial.println("Found stored name: " + storedName);
            return storedName;
        }
    }

    return "";
}

void DeviceManager::begin()
{
    randomSeed(ESP.getChipId());

    // First try to read existing name
    String storedName = readNameFromCredentials();
    if (storedName.isEmpty())
    {
        // Only generate and store a new name if no valid name exists
        deviceName = generateDeviceName();
        Serial.println("Generated new name: " + deviceName);
        updateStoredName(deviceName);
    }
    else
    {
        // Use existing name
        deviceName = storedName;
        Serial.println("Using existing name: " + deviceName);
    }
}

void DeviceManager::updateAllCredentials(JsonDocument creds)
{
    JsonDocument doc;
    if (LittleFS.exists("/credentials.json"))
    {
        File file = LittleFS.open("/credentials.json", "r");
        if (file)
        {
            DeserializationError error = deserializeJson(doc, file);
            file.close();

            if (error)
            {
                Serial.println("Failed to parse credentials file");
                return;
            }
        }
    }
    JsonDocument newDoc = creds[0];
    doc["nodeName"] = newDoc["nodeName"];
    doc["plantNodeUUID"] = newDoc["plantNodeUUID"];
    doc["connectedUserUUID"] = newDoc["connectedUserUUID"];
    doc["plantSpecies"] = newDoc["plantSpecies"];
    doc["wateringFrequencyID"] = newDoc["wateringFrequencyID"];
    doc["lightThresholdID"] = newDoc["lightThresholdID"];
    doc["humidityThresholdID"] = newDoc["humidityThresholdID"];
    doc["moistureThresholdID"] = newDoc["moistureThresholdID"];
    doc["temperatureThresholdID"] = newDoc["temperatureThresholdID"];

    File file = LittleFS.open("/credentials.json", "w");
    serializeJson(doc, file);
    file.close();

    Serial.println("Updated credentials file");
}

String DeviceManager::getSSID()
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

    if (doc.containsKey("ssid"))
    {
        String ssid = doc["ssid"].as<String>();
        Serial.println("Found stored SSID: " + ssid);
        return ssid;
    }

    return "";
}