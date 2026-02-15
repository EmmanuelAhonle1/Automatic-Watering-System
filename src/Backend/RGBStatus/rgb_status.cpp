#include "rgb_status.hpp"
#include "RGBLed.h"
#include <Arduino.h>
// #include <ESP8266WiFi.h>
#include <WiFi.h>

RGBLed rgbLed(RGB_STATUS_RED_PIN, RGB_STATUS_GREEN_PIN, RGB_STATUS_BLUE_PIN, RGBLed::COMMON_ANODE);
bool RGBStateHandler::ledState = false;

void RGBStateHandler::updateRGBStateISR()
{
    // Check WiFi Status

    if (WiFi.status() == WL_CONNECTED)
    {
        // Use cached ping status instead of pinging directly
        if (DatabaseCommands::getLastPingStatus())
        {
            currState = currentState::MYSQL_SERVER_CONNECTED;
        }
        else
        {
            currState = currentState::WIFI_CONNECTED;
        }
    }
    else if (WiFi.getMode() == WIFI_AP)
    {
        currState = currentState::AP_MODE;
    }
    else if (WiFi.status() == WL_DISCONNECTED || WiFi.status() == WL_CONNECTION_LOST)
    {
        currState = currentState::CONNECTION_LOST;
    }
    else
    {
        currState = currentState::RECONNECTING;
    }

    // Handle LED states based on current state
    switch (currState)
    {
    case currentState::WIFI_CONNECTED:
        // Slow blue blink
        ledState = !ledState;
        if (ledState)
        {
            rgbLed.brightness(20);
            rgbLed.setColor(0, 0, 255); // Blue
            setRGBColor(0, 0, 255);
        }
        else
        {
            rgbLed.off();
            setRGBColor(0, 0, 0);
        }
        break;

    case currentState::MYSQL_SERVER_CONNECTED:
        // Solid green when everything is connected
        rgbLed.brightness(20);
        rgbLed.setColor(0, 255, 0); // Green
        setRGBColor(0, 255, 0);
        break;

    case currentState::AP_MODE:
        // Solid red in AP mode
        rgbLed.brightness(20);
        rgbLed.setColor(255, 0, 0); // Red
        setRGBColor(255, 0, 0);
        break;

    case currentState::CONNECTION_LOST:
        // Fast red blink
        ledState = !ledState;
        if (ledState)
        {
            rgbLed.brightness(20);
            rgbLed.setColor(255, 0, 0); // Red
            setRGBColor(255, 0, 0);
        }
        else
        {
            rgbLed.off();
            setRGBColor(0, 0, 0);
        }
        break;

    case currentState::RECONNECTING:
        // Purple blink
        ledState = !ledState;
        if (ledState)
        {
            rgbLed.brightness(20);
            rgbLed.setColor(255, 0, 255); // Purple
            setRGBColor(255, 0, 255);
        }
        else
        {
            rgbLed.off();
            setRGBColor(0, 0, 0);
        }
        break;
    }

    prevState = currState;
}

currentState RGBStateHandler::getCurrentState()
{
    return currState;
}

void RGBStateHandler::setCurrentState(currentState state)
{
    prevState = currState;
    currState = state;
}

void RGBStateHandler::setRGBColor(int r, int g, int b)
{
    rValue = r;
    gValue = g;
    bValue = b;
}

vector<int> RGBStateHandler::getRGBColor()
{
    return {rValue, gValue, bValue};
}

bool RGBStateHandler::rgbOn()
{
    vector<int> rgbVals = getRGBColor();

    if (rgbVals[0] == 0 && rgbVals[1] == 0 && rgbVals[2] == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}