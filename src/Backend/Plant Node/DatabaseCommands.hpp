#ifndef DATABASE_COMMANDS_HPP
#define DATABASE_COMMANDS_HPP

#include <Arduino.h>
#include <string>
#include <ArduinoJson.h>

class DatabaseCommands
{
public:
    /**
     * @brief Ping the database to check if the API endpoint is reachable.
     */
    static void pingDatabase();

    /**
     * @brief Get the plant node settings using the device's MAC address.
     * @return StaticJsonDocument<512> The plant node settings.
     */
    static StaticJsonDocument<512> getPlantNodeSettings();
};

#endif // DATABASE_COMMANDS_HPP
