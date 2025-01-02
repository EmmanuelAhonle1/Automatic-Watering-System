
#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_LC709203F.h>
// Temperature and humidity sensor(DHT11)
#define DHTTYPE DHT11
#define TEMPERATURE_AND_HUMIDITY_PIN D5

// Soil moisture sensor(Capacitive Analog Soil Moisture Sensor)
#define SOIL_MOISTURE_PIN D6

// Ambient light sensor(VEM7700)
#define LIGHT_PIN D7
#define LIGHT_THRESHOLD_MIN 10000
#define LIGHT_THRESHOLD_MAX 20000

// Battery level sensor(LC709203F)
#define BATTERY_PIN D4
#define BATTERTY_SIZE LC709203F_APA_3000MAH

// RGB status LED
#define RGB_STATUS_RED_PIN D0
#define RGB_STATUS_GREEN_PIN D1
#define RGB_STATUS_BLUE_PIN D2