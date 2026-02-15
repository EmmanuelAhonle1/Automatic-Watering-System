
#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_LC709203F.h>

// Built-in LED (typically GPIO2 on ESP32 dev boards)
#define LED_BUILTIN 2

// RGB status LED (PWM capable pins)
#define RGB_STATUS_RED_PIN 25
#define RGB_STATUS_GREEN_PIN 26
#define RGB_STATUS_BLUE_PIN 27

// Soil moisture sensor (Capacitive Analog Soil Moisture Sensor)
// ADC1 pin - safe to use with WiFi (ADC2 conflicts with WiFi)
#define SOIL_MOISTURE_PIN 32

// Ambient light sensor (VEML7700) - I2C
// Using default I2C pins for ESP32
#define LIGHT_PIN_SDA 21
#define LIGHT_PIN_SCL 22
#define LIGHT_THRESHOLD_MIN 10000
#define LIGHT_THRESHOLD_MAX 20000

// Temperature and humidity sensor (DHT11)
#define DHTTYPE DHT11
#define TEMPERATURE_AND_HUMIDITY_PIN 4

// Battery level sensor (LC709203F) - I2C
// Can share I2C bus with light sensor or use different pins
#define BATTERY_PIN_SCL 22
#define BATTERY_PIN_SDA 21
#define BATTERY_SIZE LC709203F_APA_3000MAH
#define BATTERY_INT_PIN 5
