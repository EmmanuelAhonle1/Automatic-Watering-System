#include <unordered_map>
#include <string>
#include <DHT.h>
#include "dataAcquisition.hpp"
#include <gravity_soil_moisture_sensor.h>
#include <Adafruit_VEML7700.h>
#include <Adafruit_LC709203F.h>
using namespace std;

DHT dht(TEMPERATURE_AND_HUMIDITY_PIN, DHTTYPE);
GravitySoilMoistureSensor soilMoistureSensor;
Adafruit_VEML7700 ambientLightSensor = Adafruit_VEML7700();
uint16_t interruptRequest;
Adafruit_LC709203F batteryLevelSensor = Adafruit_LC709203F();
volatile bool lowBattery = false;

// Add with your other sensor functions
void checkLowBatteryStatus()
{
    // Set a global flag or take immediate action
    lowBattery = true; // Make sure to declare lowBattery as a global variable
}

void setupSensors()
{
    // Initialize the temperature and humidity sensor
    DHT dht(TEMPERATURE_AND_HUMIDITY_PIN, DHTTYPE);
    delay(10);

    // Initialize the soil moisture sensor
    soilMoistureSensor.Setup(SOIL_MOISTURE_PIN);
    delay(10);

    // Initialize the ambient light sensor
    ambientLightSensor.begin();
    delay(10);
    ambientLightSensor.setIntegrationTime(VEML7700_IT_800MS, true);
    ambientLightSensor.setLowThreshold(LIGHT_THRESHOLD_MIN);
    ambientLightSensor.setHighThreshold(LIGHT_THRESHOLD_MAX);
    ambientLightSensor.interruptEnable(true);

    // Initialize the battery level sensor

    Wire.begin(BATTERY_PIN_SDA, BATTERY_PIN_SCL);

    if (!batteryLevelSensor.begin())
    {
        Serial.println("Battery sensor not found");
        while (1)
            delay(10);
    }

    batteryLevelSensor.setThermistorB(3950);
    batteryLevelSensor.setPackSize(BATTERY_SIZE);
    batteryLevelSensor.setAlarmRSOC(20);
    attachInterrupt(digitalPinToInterrupt(BATTERY_INT_PIN), checkLowBatteryStatus, FALLING);
}

// Function to read the temperature from the soil sensor
int readTemperature()
{
    int temperature = dht.readTemperature(true);

    return temperature;
}

// Function to read the soil moisture level from the soil sensor
int readMoisture()
{
    int moisture = soilMoistureSensor.Read();

    return moisture;
}

// Placeholder function for reading temperature from another sensor
int readLight()
{
    // Implementation needed
    int ambientLight = ambientLightSensor.readLux();

    int lightPercentage = (ambientLight - LIGHT_THRESHOLD_MIN) / (LIGHT_THRESHOLD_MAX - LIGHT_THRESHOLD_MIN) * 100;

    return lightPercentage;
}

// Placeholder function for reading humidity from another sensor
int readHumidity()
{
    // Implementation needed

    int humidity = dht.readHumidity();

    return humidity;
}

uint8_t readBatteryLevel()
{
    // Implementation needed
    uint8_t batteryLevel = batteryLevelSensor.cellPercent();

    return batteryLevel;
}

void DataAcquisition::checkLowBatteryStatus()
{
    lowBattery = true;
}

bool DataAcquisition::isLowBattery() const
{
    return lowBattery;
}

unordered_map<string, int> DataAcquisition::readAllSensors()
{
    int temperature = readTemperature();
    int moisture = readMoisture();
    int lightPercentage = readLight();
    int humidity = readHumidity();
    uint8_t batteryLevel = readBatteryLevel();

    unordered_map<std::string, int> sensorData;

    sensorData["temperature"] = temperature;
    sensorData["moisture"] = moisture;
    sensorData["light"] = lightPercentage;
    sensorData["humidity"] = humidity;
    sensorData["batterylevel"] = batteryLevel;

#ifdef DEBUG_DATA_ACQUISITION
    temperature = 75;
    Serial.print("Temperature: " + String(temperature) + "°F");

    moisture = 50;
    Serial.print("Moisture: " + String(moisture) + "%");

    lightPercentage = 50;
    Serial.print("Light: " + String(lightPercentage) + "%");

    humidity = 50;
    Serial.print("Humidity: " + String(humidity) + "%");

    batteryLevel = 50;
    Serial.print("Battery Level: " + String(batteryLevel) + "%");
#endif

    return sensorData;
}