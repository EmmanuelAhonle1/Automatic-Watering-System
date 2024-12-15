

#include <unordered_map>
#include <string>

using namespace std;

// Function to read the temperature from the soil sensor
int readTemperature()
{
    int temperature = 0;

    return temperature;
}

// Function to read the soil moisture level from the soil sensor
int readMoisture()
{
    int moisture = 0;

    return moisture;
}

// Placeholder function for reading temperature from another sensor
int readLight()
{
    // Implementation needed
    int light = 0;

    return light;
}

// Placeholder function for reading humidity from another sensor
int readHumidity()
{
    // Implementation needed

    int humidity = 0;

    return humidity;
}

unordered_map<string, int> readAllSensors()
{
    int temperature = readTemperature();
    int moisture = readMoisture();
    int light = readLight();
    int humidity = readHumidity();

    unordered_map<string, int> sensorData;

    sensorData["temperature"] = temperature;
    sensorData["moisture"] = moisture;
    sensorData["light"] = light;
    sensorData["humidity"] = humidity;

    return sensorData;
}