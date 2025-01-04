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

    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error)
    {
        Serial.println("Failed to parse credentials file");
        return "";
    }

    if (doc.containsKey("plant_settings") && doc["plant_settings"].containsKey("nodeName"))
    {
        String storedName = doc["plant_settings"]["nodeName"].as<String>();
        if (storedName.isEmpty())
        {
            Serial.println("No stored name found");
        }
        else
        {
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

        StaticJsonDocument<1024> doc;
        if (LittleFS.exists("/credentials.json"))
        {
            File file = LittleFS.open("/credentials.json", "r");
            if (file)
            {
                DeserializationError error = deserializeJson(doc, file);
                file.close();
                if (error)
                {
                    Serial.println("Failed to parse existing credentials file");
                }
            }
        }

        // Update the nodeName field while preserving other fields
        JsonObject plantSettings = doc["plant_settings"];
        plantSettings["nodeName"] = deviceName;

        File file = LittleFS.open("/credentials.json", "w");
        if (!file)
        {
            Serial.println("Failed to open credentials file for writing");
            return;
        }
        serializeJson(doc, file);
        file.close();
        printCredentialsJson();
    }
    else
    {
        // Use existing name
        deviceName = storedName;
        Serial.println("Using existing name: " + deviceName);
    }
}

void DeviceManager::updateWifiSettings(const String &ssid, const String &password, const String &macAddress)
{
    printCredentialsJson();
    StaticJsonDocument<1024> doc;
    if (LittleFS.exists("/credentials.json"))
    {
        File file = LittleFS.open("/credentials.json", "r");
        if (file)
        {
            DeserializationError error = deserializeJson(doc, file);
            file.close();

            if (error)
            {
                Serial.println("Failed to parse existing credentials file");
            }
        }
    }

    // Ensure both objects exist
    if (!doc.containsKey("wifi_settings"))
    {
        doc.createNestedObject("wifi_settings"); // Changed from createNestedArray to createNestedObject
    }
    if (!doc.containsKey("plant_settings"))
    {
        doc.createNestedObject("plant_settings");
    }

    // Update wifi settings
    JsonObject wifiSettings = doc["wifi_settings"];
    wifiSettings["macAddress"] = macAddress;
    wifiSettings["ssid"] = ssid;
    wifiSettings["password"] = password;

    // Write the entire document back to file
    File file = LittleFS.open("/credentials.json", "w");
    if (!file)
    {
        Serial.println("Failed to open credentials file for writing");
        return;
    }
    serializeJson(doc, file); // Serialize the entire doc, not just wifiSettings
    file.close();

    printCredentialsJson();
}

void DeviceManager::updatePlantNodeSettings(const StaticJsonDocument<4096> &creds) // Added & to prevent copying
{
    StaticJsonDocument<4096> doc;

    // Read existing file
    if (LittleFS.exists("/credentials.json"))
    {
        File file = LittleFS.open("/credentials.json", "r");
        if (file)
        {
            DeserializationError error = deserializeJson(doc, file);
            file.close();

            if (error)
            {
                Serial.println("Failed to parse existing credentials file");
                Serial.println(error.c_str()); // Print the specific error
                return;
            }
        }
    }

    // Ensure both main objects exist
    if (!doc.containsKey("plant_settings"))
    {
        doc.createNestedObject("plant_settings");
    }
    if (!doc.containsKey("wifi_settings"))
    {
        doc.createNestedObject("wifi_settings");
    }

    // Update plant settings with null checks
    JsonObject plantSettings = doc["plant_settings"];

    // Check each field exists before updating
    if (creds.containsKey("nodeName"))
        plantSettings["nodeName"] = creds["nodeName"];
    if (creds.containsKey("plantNodeUUID"))
        plantSettings["plantNodeUUID"] = creds["plantNodeUUID"];
    if (creds.containsKey("connectedUserUUID"))
        plantSettings["connectedUserUUID"] = creds["connectedUserUUID"];
    if (creds.containsKey("plantSpecies"))
        plantSettings["plantSpecies"] = creds["plantSpecies"];
    if (creds.containsKey("wateringFrequencyID"))
        plantSettings["wateringFrequencyID"] = creds["wateringFrequencyID"];
    if (creds.containsKey("lightThresholdID"))
        plantSettings["lightThresholdID"] = creds["lightThresholdID"];
    if (creds.containsKey("humidityThresholdID"))
        plantSettings["humidityThresholdID"] = creds["humidityThresholdID"];
    if (creds.containsKey("moistureThresholdID"))
        plantSettings["moistureThresholdID"] = creds["moistureThresholdID"];
    if (creds.containsKey("temperatureThresholdID"))
        plantSettings["temperatureThresholdID"] = creds["temperatureThresholdID"];

    // Write to file
    File file = LittleFS.open("/credentials.json", "w");
    if (!file)
    {
        Serial.println("Failed to open credentials file for writing");
        return;
    }

    serializeJson(doc, file);
    file.close();

    Serial.println("Updated plant node settings in credentials file:");
    printCredentialsJson();
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

    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error)
    {
        Serial.println("Failed to parse credentials file");
        return "";
    }

    if (doc.containsKey("wifi_settings"))
    {
        return doc["wifi_settings"]["ssid"].as<String>();
    }

    return "";
}

void DeviceManager::printCredentialsJson()
{
    if (!LittleFS.exists("/credentials.json"))
    {
        Serial.println("No credentials file found");
        return;
    }

    File file = LittleFS.open("/credentials.json", "r");
    if (!file)
    {
        Serial.println("Failed to open credentials file");
        return;
    }

    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error)
    {
        Serial.println("Failed to parse credentials file");
        return;
    }

    String jsonString;
    serializeJsonPretty(doc, jsonString);
    Serial.println("Credentials JSON:");
    Serial.println(jsonString);
}