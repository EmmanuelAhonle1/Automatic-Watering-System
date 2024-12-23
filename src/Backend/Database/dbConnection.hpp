#ifndef DATABASE_CONNECTION_HPP
#define DATABASE_CONNECTION_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <WiFiClient.h>

using namespace std;

class DatabaseConnection
{
private:
    // Connection settings
    string host;
    string user;
    string password;
    string api_url;
    WiFiClient client;

    /**
     * @brief Send an HTTP GET request
     * @param url The URL to send the request to.
     * @return string The response from the server.
     */
    string sendGetRequest(const string &url);

public:
    /**
     * @brief Construct a new Database Connection object
     * Initializes the connection settings.
     */
    DatabaseConnection();

    /**
     * @brief Execute a query on the MySQL server
     * Executes the provided SQL query and returns the result as a vector of unordered maps.
     * @param query The SQL query to execute.
     * @return vector<unordered_map<string, string>> The result of the query.
     */
    vector<unordered_map<string, string>> executeQuery(string query);

    /**
     * @brief Get the name of a plant node from the API based on its MAC address
     * Retrieves the name of a plant node with the given MAC address.
     * @return string The name of the plant node.
     */
    string getPlantNodeName();
};

#endif // DATABASE_CONNECTION_HPP