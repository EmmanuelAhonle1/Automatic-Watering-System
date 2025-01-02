#include "dbConnection.hpp"
#include "../Credentials/credentials.h"
#include <Esp8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>
using namespace std;
#include <LittleFS.h>
#include "../DB Query Creator/dbQueryCreator.hpp"

JsonDocument getPlantNodeSettings()
{
    URLQueryBuilder queryBuilder(USER, PASSWORD);

    string urlEndpoint = "/plantNode/select";

    queryBuilder.addParameter("macAddress", WiFi.macAddress().c_str());
    queryBuilder.setReturnFields({"nodeName", "plantNodeUUID", "connectedUserUUID",
                                  "plantSpecies", "wateringFrequencyID", "lightThresholdID",
                                  "humidityThresholdID", "moistureThresholdID", "temperatureThresholdID"});

    string response = sendGetRequest(queryBuilder.formURLWithEndpoint(urlEndpoint), queryBuilder.build());
    if (response.empty())
    {
        Serial.println("Empty response received");
        return JsonDocument();
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, response.c_str());
    if (error)
    {
        Serial.printf("deserializeJson() failed: %s\n", error.c_str());
        return JsonDocument();
    }

    if (!doc.containsKey("error"))
    {
        return doc;
    }
    return JsonDocument();
}

bool pingDatabase()
{
    string endpoint = "/checkConnection";
    string query = "";
    string response = sendGetRequest(endpoint, query);

    JsonDocument doc;

    deserializeJson(doc, response.c_str());

    if (doc["error"])
    {
        Serial.println("Database connection failed");
        return false;
    }
    else
    {

        Serial.println("Database connection successful");
        Serial.println(response.c_str());
    }
    return true;
}