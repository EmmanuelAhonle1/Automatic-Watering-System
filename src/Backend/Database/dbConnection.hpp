#ifndef DB_CONNECTION_HPP
#define DB_CONNECTION_HPP

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class DatabaseConnection
{
public:
    DatabaseConnection();
    bool checkConnection();
    vector<unordered_map<string, string>> executeQuery(string query);
    string sendGetRequest(const string &url);
    string sendPostRequest(unordered_map<string, string> data); // Updated method signature
    string getPlantNodeName();

private:
    string host;
    string user;
    string password;
    string api_url;
    string urlEncode(const char *str); // Declare urlEncode function
};

#endif // DB_CONNECTION_HPP