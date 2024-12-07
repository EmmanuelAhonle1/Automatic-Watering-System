#pragma once
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include "device_manager.hpp"

class OTAManager
{
private:
    DeviceManager *deviceManager;

public:
    OTAManager(DeviceManager *dm) : deviceManager(dm) {}

    void begin()
    {
        // Configure OTA
        ArduinoOTA.setHostname(deviceManager->getName().c_str());

        ArduinoOTA.onStart([]()
                           { Serial.println("OTA: Start"); });

        ArduinoOTA.onEnd([]()
                         { Serial.println("\nOTA: End"); });

        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                              { Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100))); });

        ArduinoOTA.onError([](ota_error_t error)
                           {
            Serial.printf("OTA Error[%u]: ", error);
            switch(error) {
                case OTA_AUTH_ERROR: 
                    Serial.println("Auth Failed");
                    break;
                case OTA_BEGIN_ERROR: 
                    Serial.println("Begin Failed");
                    break;
                case OTA_CONNECT_ERROR: 
                    Serial.println("Connect Failed");
                    break;
                case OTA_RECEIVE_ERROR: 
                    Serial.println("Receive Failed");
                    break;
                case OTA_END_ERROR: 
                    Serial.println("End Failed");
                    break;
            } });

        ArduinoOTA.begin();
        Serial.println("OTA manager initialized");

        // Set up MDNS responder
        if (MDNS.begin(deviceManager->getName().c_str()))
        {
            Serial.println("MDNS responder started");
            MDNS.addService("http", "tcp", 80);
            MDNS.addService("ota", "udp", 8266);
        }
    }

    void handle()
    {
        ArduinoOTA.handle();
        MDNS.update();
    }
};