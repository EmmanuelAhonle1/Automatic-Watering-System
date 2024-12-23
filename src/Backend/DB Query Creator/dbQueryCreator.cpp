#include "dbQueryCreator.hpp"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <string.h>

using namespace std;

string sendGetRequest(const string &url)
{
    WiFiClientSecure client;
    HTTPClient http;
    string payload = "";

    client.setInsecure(); // Required for HTTPS

    if (http.begin(client, url.c_str()) && WiFi.status() == WL_CONNECTED)
    {
        http.setTimeout(10000);
        client.setTimeout(10000);

        int httpCode = http.GET();
        if (httpCode > 0)
        {
            payload = http.getString().c_str();
#ifdef DEBUG_DBQC
            Serial.println(payload.c_str());
#endif
        }
        else
        {
            Serial.printf("GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }

    return payload;
}

bool pingDatabase()
{
    string baseUrl = API_URL;
    string url = baseUrl + "/checkConnection";
    string response = sendGetRequest(url);

    JsonDocument doc;

    deserializeJson(doc, response.c_str());

    if (doc["error"])
    {
        Serial.println("Database connection failed");
        return false;
    }
    else
    {

#ifdef DEBUG_DBQC
        Serial.println("Database connection successful");
        Serial.println(response.c_str());
#endif
    }
    return true;
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
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : str)
    {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            escaped << c;
        }
        else
        {
            escaped << '%' << std::setw(2) << int((unsigned char)c);
        }
    }

    return escaped.str();
}

std::string URLQueryBuilder::build() const
{
    std::ostringstream url;
    url << baseUrl;

    bool firstParam = true;
    for (const auto &param : parameters)
    {
        for (const auto &value : param.second)
        {
            url << (firstParam ? "?" : "&");
            url << urlEncode(param.first);
            if (param.second.size() > 1)
            {
                url << "[]";
            }
            url << "=" << urlEncode(value);
            firstParam = false;
        }
    }

    if (!returnFields.empty())
    {
        url << (firstParam ? "?" : "&") << "fields=";
        for (size_t i = 0; i < returnFields.size(); ++i)
        {
            if (i > 0)
                url << ",";
            url << urlEncode(returnFields[i]);
        }
    }

    return url.str();
}
