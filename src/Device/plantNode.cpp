

#include <string>
#include <chrono>
#include <ctime>
#include <vector>

using namespace std;
using namespace chrono;

// class DatabaseConnection
// {
// private:
//     sql::Driver *driver;
//     std::unique_ptr<sql::Connection> connection;
//     std::unique_ptr<sql::Statement> statement;

//     // Connection settings
//     std::string host;
//     std::string user;
//     std::string password;
//     std::string database;

// public:
//     DatabaseConnection(std::string host, std::string user, std::string password, std::string database)
//     {
//         this->host = host;
//         this->user = user;
//         this->password = password;
//         this->database = database;
//     }

//     void connect()
//     {

//         driver = get_driver_instance();
//         connection.reset(driver->connect(host, user, password));
//         connection->setSchema(database);
//         statement.reset(connection->createStatement());
//     }

//     void disconnect()
//     {
//         connection->close();
//     }

//     void executeQuery(std::string query)
//     {
//         statement->execute(query);
//     }
// };

class dateTime
{
private:
    string time;

public:
    dateTime()
    {
        system_clock::time_point b = system_clock::now();

        time_t c = system_clock::to_time_t(b);
        time = ctime(&c);
    }

    operator string() const
    {
        return time;
    }
};

class PlantNode
{
private:
    int plantNode_uuid;
    int user_uuid;
    string nodeName;
    string plantSpecies;
    string wateringFrequency;
    int soilMoisture;
    string lastWatered;
    string location;

public:
    PlantNode(int newUUID) : plantNode_uuid(newUUID)
    {
        if (1 /*uuid exists in database*/)
        {
            // load data from database
        }
        else
        {
            // create new plant node
        }
    }
};

class PlantNodeManager
{
private:
    vector<PlantNode> plantNodes;
};