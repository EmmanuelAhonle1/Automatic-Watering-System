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
     * @brief Read the stored device name from the credentials file.
     *
     * @return String The stored device name.
     */
    String readNameFromCredentials();

    /**
     * @brief Initialize the device manager.
     */
    void begin();

    /**
     * @brief Update plant node settings in the credentials file.
     *
     * @param creds The JSON document containing the plant node settings to update.
     */
    void updatePlantNodeSettings(const StaticJsonDocument<512> &creds);

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

    /**
     * @brief Update WiFi settings in the credentials file.
     *
     * @param ssid The SSID of the network.
     * @param password The password of the network.
     * @param macAddress The MAC address of the device.
     */
    void updateWifiSettings(const String &ssid, const String &password, const String &macAddress);

    /**
     * @brief Print the JSON content of the credentials file.
     */
    void printCredentialsJson();

private:
    String deviceName;
};

#endif // DEVICE_MANAGER_HPP