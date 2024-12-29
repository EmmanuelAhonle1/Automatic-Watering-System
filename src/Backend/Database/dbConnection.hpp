#ifndef DATABASE_CONNECTION_HPP
#define DATABASE_CONNECTION_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <WiFiClient.h>
#include <ArduinoJson.h>

using namespace std;

/**
 * @brief Get the settings of a plant node from the API based on its MAC address
 *
 * @return JsonDocument The settings of the plant node.
 */
JsonDocument getPlantNodeSettings();

/**
 * @brief Ping the database to check the connection.
 *
 * @return bool True if the connection is successful, false otherwise.
 */
bool pingDatabase();

#endif // DATABASE_CONNECTION_HPP