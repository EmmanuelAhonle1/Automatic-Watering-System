#include "dbConnection.hpp"
// #include "../Credentials/credentials.h"
#include <Esp8266WiFi.h>
#define SERVER_HOST "automatic-watering-system.cdq4ye8ggh8i.us-east-1.rds.amazonaws.com"
#define USER "admin"
#define PASSWORD "Mighty_Duck1738"

DatabaseConnection::DatabaseConnection() : conn((Client *)&client)
{
    // MySQL server settings
    host = SERVER_HOST;
    user = USER;
    password = PASSWORD;
    cur = nullptr;
}

void DatabaseConnection::connect()
{
    WiFi.status() == WL_CONNECTED ? digitalWrite(LED_BUILTIN, LOW) : digitalWrite(LED_BUILTIN, HIGH);

    if (conn.connect(SERVER_HOST, 3306, USER, PASSWORD))
    {
        delay(500);
        Serial.println("Connected to MySQL server");
    }
    else
    {
        Serial.println("Connection failed.");
    }
}

void DatabaseConnection::disconnect()
{
    if (cur != nullptr)
    {
        delete cur;
        cur = nullptr;
    }
}

std::vector<std::unordered_map<std::string, std::string>> DatabaseConnection::executeQuery(std::string query)
{
    row_values *row = NULL;
    std::vector<std::unordered_map<std::string, std::string>> result;

    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    cur_mem->execute(query.c_str());

    column_names *columns = cur_mem->get_columns();

    do
    {
        row = cur_mem->get_next_row();
        if (row != NULL)
        {
            std::unordered_map<std::string, std::string> row_map;
            for (int i = 0; i < columns->num_fields; i++)
            {
                row_map[columns->fields[i]->name] = row->values[i];
            }

            result.push_back(row_map);
        }

    } while (row != NULL);

    delete cur_mem;

    cur->close();

    return result;
}