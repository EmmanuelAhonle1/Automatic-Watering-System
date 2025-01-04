#include "DatabaseCommands.hpp"
#include "../DB Query Creator/dbQueryCreator.hpp"
#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

bool DatabaseCommands::pingDatabase()
{
    const std::string endpoint = "/";
    const std::string query = "";

    URLQueryBuilder queryBuilder("", ""); // No authentication needed for this example
    std::string response = sendGetRequest(endpoint, queryBuilder);

    if (!response.empty())
    {
        Serial.println("Database ping successful: " + String(response.c_str()));
        return true;
    }
    else
    {
        Serial.println("Database ping failed.");
        return false;
    }
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
