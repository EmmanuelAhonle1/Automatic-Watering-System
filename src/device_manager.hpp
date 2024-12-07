#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

class DeviceManager
{
private:
    String deviceName;
    String generateDeviceName();
    String readNameFromCredentials();

public:
    void begin();
    void setupAccessPoint();
    String getName() { return deviceName; }
};

extern DeviceManager deviceManager;

#endif // DEVICE_MANAGER_HPP