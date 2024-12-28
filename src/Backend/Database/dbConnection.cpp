#include "dbConnection.hpp"
#include "../Credentials/credentials.h"
#include <Esp8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
using namespace std;
#include <LittleFS.h>
#include "../DB Query Creator/dbQueryCreator.hpp"

string getPlantNodeName()
{
    URLQueryBuilder queryBuilder(USER, PASSWORD);

    string urlEndpoint = "/plantNode/select";

    queryBuilder.addParameter("macAddress", WiFi.macAddress().c_str());
    queryBuilder.setReturnFields({"nodeName"});

    string response = sendGetRequest(urlEndpoint, queryBuilder.build());
    if (response.empty() && WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Empty response received");
        return "";
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, response.c_str());

        if (error)
        {
            Serial.printf("deserializeJson() failed: %s\n", error.c_str());
            return "";
        }

        if (!doc["error"])
        {
            return doc["nodeName"].as<string>();
        }
    }
    return "";
}