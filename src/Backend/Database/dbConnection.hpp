#ifndef DATABASE_CONNECTION_HPP
#define DATABASE_CONNECTION_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <WiFiClient.h>

using namespace std;

/**
 * @brief Get the name of a plant node from the API based on its MAC address
 *
 * @return string The name of the plant node.
 */
string getPlantNodeName();

#endif // DATABASE_CONNECTION_HPP