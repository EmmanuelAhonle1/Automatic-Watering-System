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
    bool updateStoredName(const String &newName);
    String getName() { return deviceName; }
    bool regenerateName()
    {
        String newName = generateDeviceName();
        if (updateStoredName(newName))
        {
            deviceName = newName;
            return true;
        }
        return false;
    }
};

extern DeviceManager deviceManager;

#endif // DEVICE_MANAGER_HPP