#ifndef RGB_STATUS_HPP
#define RGB_STATUS_HPP

#include "../Device/Pinouts.hpp"
#include <RGBLed.h>
#include <Arduino.h>
#include <string>
#include <vector>
#include "../Plant Node/DatabaseCommands.hpp"
using namespace std;
extern RGBLed rgbLed;

enum class currentState
{
    WIFI_CONNECTED,
    AP_MODE,
    MYSQL_SERVER_CONNECTED,
    CONNECTION_LOST,
    RECONNECTING
};
// In rgb_status.hpp
class RGBStateHandler
{
public:
    void updateRGBStateISR();
    void setCurrentState(currentState state);
    currentState getCurrentState();

private:
    currentState prevState;
    currentState currState;
    static bool ledState;
    void setRGBColor(int r, int g, int b);
    vector<int> getRGBColor();
    bool rgbOn();

    int rValue = 0;
    int gValue = 0;
    int bValue = 0;
};

#endif
