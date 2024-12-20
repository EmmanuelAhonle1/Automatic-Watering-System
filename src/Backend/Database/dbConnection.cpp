#include "dbConnection.hpp"
#include "../Credentials/credentials.h"
#include <Esp8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
using namespace std;

DatabaseConnection::DatabaseConnection()
{
    // MySQL server settings
    host = SERVER_HOST;
    user = USER;
    password = PASSWORD;
    api_url = API_URL;
}

bool DatabaseConnection::checkConnection()
{
    WiFi.status() == WL_CONNECTED ? digitalWrite(LED_BUILTIN, LOW) : digitalWrite(LED_BUILTIN, HIGH);
    return WiFi.status() == WL_CONNECTED;
}

vector<unordered_map<string, string>> DatabaseConnection::executeQuery(string query)
{
    vector<unordered_map<string, string>> result = vector<unordered_map<string, string>>();

    return result;
}

string DatabaseConnection::sendGetRequest(const string &url)
{
    WiFiClientSecure client;
    HTTPClient http;
    string payload = "";
    checkConnection();

    client.setInsecure(); // Required for HTTPS

    if (http.begin(client, url.c_str()) && WiFi.status() == WL_CONNECTED)
    {
        http.setTimeout(10000);
        client.setTimeout(10000);

        int httpCode = http.GET();
        if (httpCode > 0)
        {
            payload = http.getString().c_str();
            Serial.println(payload.c_str());
        }
        else
        {
            Serial.printf("GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
    else
    {
        Serial.println("Unable to connect");
    }

    return payload;
}

string DatabaseConnection::getPlantNodeName()
{
    string url = api_url + "plantNode/" + WiFi.macAddress().c_str();
    string response = sendGetRequest(url);

    if (response.empty() && WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Empty response received");
        return "";
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, response.c_str());

        if (error)
        {
            Serial.printf("deserializeJson() failed: %s\n", error.c_str());
            return "";
        }

        if (!doc["error"])
        {
            return doc["name"].as<string>();
        }
        else
        {
            Serial.printf("Plant not found: %s\n", response.c_str());
        }
    }

    return "";
}