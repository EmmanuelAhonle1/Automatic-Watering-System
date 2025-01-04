#ifndef DATABASE_COMMANDS_HPP
#define DATABASE_COMMANDS_HPP

#include <Arduino.h>
#include <string>
#include <ArduinoJson.h>

#define DEBUG_DB_COMMANDS // Enable debug mode

class DatabaseCommands
{
private:
    static bool lastPingStatus;
    static unsigned long lastPingTime;
    static const unsigned long PING_INTERVAL = 5000; // 5 seconds between pings

public:
    /**
     * @brief Ping the database
     *
     * @return true if ping successful, false otherwise
     */
    static bool pingDatabase();

    /**
     * @brief Get the last ping status
     *
     * @return true if last ping was successful, false otherwise
     */
    static bool getLastPingStatus() { return lastPingStatus; }

    /**
     * @brief Get the plant node settings
     *
     * @return StaticJsonDocument<512> The plant node settings
     */
    static StaticJsonDocument<512> getPlantNodeSettings();
};

#endif // DATABASE_COMMANDS_HPP
