#ifndef DB_QUERY_CREATOR_HPP
#define DB_QUERY_CREATOR_HPP

#define DEBUG_DBQC // Enable debug mode

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "../Credentials/credentials.h" // Include credentials
using namespace std;

/**
 * @brief Class for building URL queries with parameters and return fields.
 */
class URLQueryBuilder
{
public:
    /**
     * @brief Construct a new URLQueryBuilder object
     *
     * @param user The username for authentication.
     * @param password The password for authentication.
     */
    URLQueryBuilder(const std::string &user, const std::string &password);

    /**
     * @brief Add a single parameter to the query.
     *
     * @param key The parameter key.
     * @param value The parameter value.
     */
    void addParameter(const std::string &key, const std::string &value);

    /**
     * @brief Add multiple values for a parameter.
     *
     * @param key The parameter key.
     * @param values The parameter values.
     */
    void addParameters(const std::string &key, const std::vector<std::string> &values);

    /**
     * @brief Set the fields to return in the query.
     *
     * @param fields The fields to return.
     */
    void setReturnFields(const std::vector<std::string> &fields);

    /**+
     * @brief Build the complete URL with parameters and return fields.
     *
     * @return std::string The complete URL.
     */
    std::string build() const;

private:
    std::string baseUrl = API_URL;                              // Base URL for the API
    std::string user;                                           // Username for authentication
    std::string password;                                       // Password for authentication
    std::map<std::string, std::vector<std::string>> parameters; // Query parameters
    std::vector<std::string> returnFields;                      // Fields to return

    /**
     * @brief URL encode a string.
     *
     * @param str The string to encode.
     * @return std::string The URL encoded string.
     */
    static std::string urlEncode(const std::string &str);
};

/**
 * @brief Send an HTTP GET request.
 *
 * @param url The URL to send the request to.
 * @return string The response from the server.
 */
string sendGetRequest(const string &url);

/**
 * @brief Ping the database to check the connection.
 *
 * @return bool True if the connection is successful, false otherwise.
 */
bool pingDatabase();

#endif // DB_QUERY_CREATOR_HPP