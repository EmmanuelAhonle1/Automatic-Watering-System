#include "rgb_status.hpp"
#include "RGBLed.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>

RGBLed rgbLed(RGB_STATUS_RED_PIN, RGB_STATUS_GREEN_PIN, RGB_STATUS_BLUE_PIN, RGBLed::COMMON_ANODE);

void RGBStateHandler::handleState(currentState state)
{
    // if (state != prevState)
    // {
    //     switch (state)
    //     {
    //     case currentState::WIFI_CONNECTED:
    //         rgbLed.brightness(20);
    //         rgbLed.setColor(0, 0, 255);
    //         break;
    //     case currentState::AP_MODE:
    //         rgbLed.brightness(20);
    //         rgbLed.setColor(255, 0, 0);
    //         break;
    //     case currentState::MYSQL_SERVER_CONNECTED:
    //         rgbLed.brightness(20);
    //         rgbLed.setColor(0, 255, 0);
    //         break;

    //     case currentState::RECONNECTING:
    //         rgbLed.brightness(20);
    //         rgbLed.flash(255, 0, 255, 100);
    //         break;
    //         prevState = state;
    //     }
    // }

    if (WiFi.status() == WL_CONNECTED)
    {
        rgbLed.brightness(20);
        rgbLed.setColor(0, 0, 255);
    }
    else
    {
        rgbLed.brightness(20);
        rgbLed.setColor(255, 0, 0);
    }
}
