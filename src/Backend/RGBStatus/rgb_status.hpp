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

class RGBStateHandler
{
public:
    /**
     * @brief Update the RGB state in an ISR
     */
    void updateRGBStateISR();

    /**
     * @brief Set the current state
     * 
     * @param state The new state
     */
    void setCurrentState(currentState state);

    /**
     * @brief Get the current state
     * 
     * @return currentState The current state
     */
    currentState getCurrentState();

private:
    currentState prevState;
    currentState currState;
    static bool ledState;

    /**
     * @brief Set the RGB color
     * 
     * @param r Red value
     * @param g Green value
     * @param b Blue value
     */
    void setRGBColor(int r, int g, int b);

    /**
     * @brief Get the RGB color
     * 
     * @return vector<int> The RGB color values
     */
    vector<int> getRGBColor();

    /**
     * @brief Check if the RGB is on
     * 
     * @return true if RGB is on, false otherwise
     */
    bool rgbOn();

    int rValue = 0;
    int gValue = 0;
    int bValue = 0;
};

#endif // RGB_STATUS_HPP
