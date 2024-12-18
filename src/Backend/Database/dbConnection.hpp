#ifndef DATABASE_CONNECTION_HPP
#define DATABASE_CONNECTION_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <WiFiClient.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

class DatabaseConnection
{
private:
    // Connection settings
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    WiFiClient client;
    MySQL_Connection conn;
    MySQL_Cursor *cur;

public:
    /**
     * @brief Construct a new Database Connection object
     * Initializes the connection settings.
     */
    DatabaseConnection();

    /**
     * @brief Connect to the MySQL server
     * Establishes a connection to the MySQL server using the provided credentials.
     */
    void connect();

    /**
     * @brief Disconnect from the MySQL server
     * Cleans up the cursor and disconnects from the MySQL server.
     */
    void disconnect();

    /**
     * @brief Execute a query on the MySQL server
     * Executes the provided SQL query and returns the result as a vector of unordered maps.
     * @param query The SQL query to execute.
     * @return std::vector<std::unordered_map<std::string, std::string>> The result of the query.
     */
    std::vector<std::unordered_map<std::string, std::string>> executeQuery(std::string query);
};

#endif // DATABASE_CONNECTION_HPP