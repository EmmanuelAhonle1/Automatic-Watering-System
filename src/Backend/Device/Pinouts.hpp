
#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_LC709203F.h>

// RGB status LED
#define RGB_STATUS_RED_PIN D0
#define RGB_STATUS_GREEN_PIN D1
#define RGB_STATUS_BLUE_PIN D2

// Soil moisture sensor(Capacitive Analog Soil Moisture Sensor)
#define SOIL_MOISTURE_PIN D3

// Ambient light sensor(VEM7700)
#define LIGHT_PIN_SDA D4
#define LIGHT_PIN_SCL D9
#define LIGHT_THRESHOLD_MIN 10000
#define LIGHT_THRESHOLD_MAX 20000

// Temperature and humidity sensor(DHT11)
#define DHTTYPE DHT11
#define TEMPERATURE_AND_HUMIDITY_PIN D5

// Battery level sensor(LC709203F)
#define BATTERY_PIN_SCL D6
#define BATTERY_PIN_SDA D7
#define BATTERY_SIZE LC709203F_APA_3000MAH
#define BATTERY_INT_PIN D8
