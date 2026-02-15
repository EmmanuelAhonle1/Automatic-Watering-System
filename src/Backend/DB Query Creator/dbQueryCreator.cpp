#include "dbQueryCreator.hpp"
#include <WiFi.h>
#include <HTTPClient.h>
// #include <ESP8266WiFi.h>
// #include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <string.h>

using namespace std;

const string baseUrl = API_URL; // Define your base URL here

string sendGetRequest(const string &endpoint, const URLQueryBuilder &queryBuilder)
{
    string response = "";
    { // Create scope for client objects
        WiFiClientSecure client;
        HTTPClient http;

        client.setTimeout(5000);
        http.setTimeout(5000);
        client.setInsecure();

        string url = queryBuilder.build(endpoint);
        Serial.println(url.c_str());
        if (http.begin(client, url.c_str()))
        {
            int httpCode = http.GET();
            if (httpCode > 0)
            {
                response = http.getString().c_str();
            }
            http.end();
        }
        // Client and HTTP objects will be destroyed here
    }
    return response;
}

string sendPostRequest(const string &endpoint, const URLQueryBuilder &queryBuilder)
{
    WiFiClientSecure client;
    HTTPClient http;
    string payload = "";

    string url = queryBuilder.build(endpoint);

    client.setInsecure(); // Required for HTTPS

    if (http.begin(client, url.c_str()) && WiFi.status() == WL_CONNECTED)
    {
        http.setTimeout(10000);
        client.setTimeout(10000);

        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int httpCode = http.POST(queryBuilder.getQueryString().c_str());
        if (httpCode > 0)
        {
            payload = http.getString().c_str();
#ifdef DEBUG_DBQC
            Serial.println(payload.c_str());
#endif
        }
        else
        {
            Serial.printf("POST request failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }

    return payload;
}

// URLQueryBuilder class implementation
URLQueryBuilder::URLQueryBuilder(const std::string &user, const std::string &password)
    : user(user), password(password) {}

void URLQueryBuilder::addParameter(const std::string &key, const std::string &value)
{
    parameters[key].push_back(value);
}

void URLQueryBuilder::addParameters(const std::string &key, const std::vector<std::string> &values)
{
    parameters[key] = values;
}

void URLQueryBuilder::setReturnFields(const std::vector<std::string> &fields)
{
    returnFields = fields;
}

std::string URLQueryBuilder::urlEncode(const std::string &str)
{
    std::string escaped;
    char hex[4];
    for (char c : str)
    {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            escaped += c;
        }
        else
        {
            snprintf(hex, sizeof(hex), "%%%02X", (unsigned char)c);
            escaped += hex;
        }
    }
    return escaped;
}

std::string URLQueryBuilder::build(const std::string &endpoint) const
{
    std::string url = baseUrl + endpoint;
    bool firstParam = true;
    for (const auto &param : parameters)
    {
        for (const auto &value : param.second)
        {
            url += (firstParam ? "?" : "&");
            url += urlEncode(param.first);
            if (param.second.size() > 1)
            {
                url += "[]";
            }
            url += "=" + urlEncode(value);
            firstParam = false;
        }
    }

    if (!returnFields.empty())
    {
        url += (firstParam ? "?" : "&") + std::string("fields=");
        for (size_t i = 0; i < returnFields.size(); ++i)
        {
            if (i > 0)
                url += ",";
            url += urlEncode(returnFields[i]);
        }
    }

    return url;
}

std::string URLQueryBuilder::getQueryString() const
{
    std::string queryString;
    bool firstParam = true;
    for (const auto &param : parameters)
    {
        for (const auto &value : param.second)
        {
            queryString += (firstParam ? "" : "&");
            queryString += urlEncode(param.first);
            if (param.second.size() > 1)
            {
                queryString += "[]";
            }
            queryString += "=" + urlEncode(value);
            firstParam = false;
        }
    }

    if (!returnFields.empty())
    {
        queryString += (firstParam ? "" : "&") + std::string("fields=");
        for (size_t i = 0; i < returnFields.size(); ++i)
        {
            if (i > 0)
                queryString += ",";
            queryString += urlEncode(returnFields[i]);
        }
    }

    return queryString;
}