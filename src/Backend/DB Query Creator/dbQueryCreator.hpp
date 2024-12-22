#ifndef DB_QUERY_CREATOR_HPP
#define DB_QUERY_CREATOR_HPP

// #define DEBUG_DBQC // Enable debug mode

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "../Credentials/credentials.h" // Include credentials
using namespace std;

class URLQueryBuilder
{
public:
    URLQueryBuilder(const std::string &user, const std::string &password); // Updated constructor
    void addParameter(const std::string &key, const std::string &value);
    void addParameters(const std::string &key, const std::vector<std::string> &values);
    void setReturnFields(const std::vector<std::string> &fields);
    std::string build() const;
    string sendGetRequest(const string &url); // Added sendGetRequest method

private:
    std::string baseUrl = API_URL;
    std::string user;
    std::string password;
    std::map<std::string, std::vector<std::string>> parameters;
    std::vector<std::string> returnFields;
    static std::string urlEncode(const std::string &str); // URL encode method
};

class SQLQueryBuilder
{
public:
    SQLQueryBuilder(const std::string &tableName);
    std::pair<std::string, std::vector<std::string>> buildQuery(
        const std::map<std::string, std::vector<std::string>> &parameters,
        const std::vector<std::string> &returnFields);
    string sendGetRequest(const string &url); // Added sendGetRequest method
    bool pingDatabase();                      // Declare pingDatabase function

private:
    std::string tableName;
    static std::string sanitizeIdentifier(const std::string &identifier); // Sanitize identifier method
};

#endif // DB_QUERY_CREATOR_HPP