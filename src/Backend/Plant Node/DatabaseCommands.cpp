#include "DatabaseCommands.hpp"
#include "../DB Query Creator/dbQueryCreator.hpp"
#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

bool DatabaseCommands::lastPingStatus = false;
unsigned long DatabaseCommands::lastPingTime = 0;

bool DatabaseCommands::pingDatabase()
{
    unsigned long currentTime = millis();

    // Only ping if enough time has elapsed since last ping
    if (currentTime - lastPingTime >= PING_INTERVAL)
    {
        lastPingTime = currentTime;

        const std::string endpoint = "/";
        URLQueryBuilder queryBuilder("", "");
        std::string response = sendGetRequest(endpoint, queryBuilder);

        lastPingStatus = !response.empty();

#ifdef DEBUG_DB_COMMANDS
        if (lastPingStatus)
        {
            Serial.println("Database ping successful");
        }
        else
        {
            Serial.println("Database ping failed");
        }
#endif
    }

    return lastPingStatus;
}

StaticJsonDocument<512> DatabaseCommands::getPlantNodeSettings()
{
    StaticJsonDocument<512> plantNodeSettings;

    // Get the MAC address of the device
    String macAddress = WiFi.macAddress();

    // Build the query to select the plant node settings
    URLQueryBuilder queryBuilder("", ""); // No authentication needed for this example
    queryBuilder.addParameter("macAddress", macAddress.c_str());
    queryBuilder.setReturnFields({"nodeName", "plantNodeUUID", "plantSpecies", "connectedUserUUID", "wateringFrequencyID", "lightThresholdID", "humidityThresholdID", "moistureThresholdID", "temperatureThresholdID"});

    std::string response = sendGetRequest("/plantNode/select", queryBuilder);

    if (!response.empty())
    {
        DeserializationError error = deserializeJson(plantNodeSettings, response);
        if (error)
        {
            Serial.println("Failed to parse JSON response");
        }
    }
    else
    {
        Serial.println("Failed to retrieve plant node settings");
    }

    return plantNodeSettings;
}
