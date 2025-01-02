#ifndef RGB_STATUS_HPP
#define RGB_STATUS_HPP

#include "../Device/Pinouts.hpp"
#include <RGBLed.h>
#include <Arduino.h>
#include <string>
extern RGBLed rgbLed;

enum class currentState
{
    WIFI_CONNECTED,
    AP_MODE,
    MYSQL_SERVER_CONNECTED,
    CONNECTION_LOST,
    RECONNECTING
};
class RGBStateHandler
{
public:
    void handleState(currentState state);

private:
    currentState prevState;
};

#endif
