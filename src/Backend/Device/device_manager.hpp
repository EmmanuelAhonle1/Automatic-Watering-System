#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

class DeviceManager
{
public:
    /**
     * @brief Generate a random device name.
     *
     * @return String The generated device name.
     */
    String generateDeviceName();

    /**
     * @brief Update the stored device name in the credentials file.
     *
     * @param newName The new device name.
     * @return bool True if the update was successful, false otherwise.
     */
    bool updateStoredName(const String &newName);

    /**
     * @brief Read the device name from the credentials file.
     *
     * @return String The stored device name.
     */
    String readNameFromCredentials();

    /**
     * @brief Initialize the device manager.
     */
    void begin();

    /**
     * @brief Update all credentials in the credentials file.
     *
     * @param creds The JSON document containing the credentials to update.
     */
    void updateAllCredentials(JsonDocument creds);

    /**
     * @brief Get the SSID from the credentials file.
     *
     * @return String The stored SSID.
     */
    String getSSID();

    /**
     * @brief Get the device name.
     *
     * @return String The device name.
     */
    String getDeviceName() { return deviceName; };

private:
    String deviceName;
};

#endif // DEVICE_MANAGER_HPP