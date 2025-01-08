#ifndef DATA_ACQUISITION_HPP
#define DATA_ACQUISITION_HPP

#define DEBUG_DATA_ACQUISITION // Enable debug mode

#include "Pinouts.hpp"
#include <Arduino.h>
#include <DHT.h> // Include DHT sensor library
#include <Wire.h>
#include "Adafruit_LC709203F.h"
class DataAcquisition
{
public:
    /**
     * @brief Initialize the data acquisition system
     * Sets up the necessary pins and configurations.
     */
    void begin();

    /**
     * @brief Read soil moisture level
     * Reads the soil moisture level from the sensor.
     * @return int Soil moisture level
     */
    int readSoilMoisture();

    /**
     * @brief Read temperature
     * Reads the temperature from the sensor.
     * @return float Temperature in degrees Celsius
     */
    float readTemperature();

    /**
     * @brief Read humidity
     * Reads the humidity from the sensor.
     * @return float Humidity percentage
     */
    float readHumidity();

    /**
     * @brief Check if the battery is low
     * Updates the lowBattery status.
     */
    void checkLowBatteryStatus();

    bool isLowBattery() const;

private:
    Adafruit_LC709203F batteryLevelSensor;
};

#endif // DATA_ACQUISITION_HPP