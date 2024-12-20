#include <string>
#include <chrono>
#include <ctime>
#include <vector>
#include <ArduinoJson.h>

#include <Ethernet.h>

#include "../Credentials/credentials.h"
using namespace std;

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
    bool currentlyWatering = false;
    string location;

public:
    PlantNode(int pnUUID, int userUUID, string nName, string pSpecies, string wF, int sM, string lW, string loc)
        : plantNode_uuid(pnUUID),
          user_uuid(userUUID),
          nodeName(nName),
          plantSpecies(pSpecies),
          wateringFrequency(wF),
          soilMoisture(sM),
          lastWatered(lW),
          location(loc)
    {
    }

    // Getters
    int getPlantNodeUUID() const { return plantNode_uuid; }
    int getUserUUID() const { return user_uuid; }
    string getNodeName() const { return nodeName; }
    string getPlantSpecies() const { return plantSpecies; }
    string getWateringFrequency() const { return wateringFrequency; }
    int getSoilMoisture() const { return soilMoisture; }
    string getLastWatered() const { return lastWatered; }
    string getLocation() const { return location; }
    bool isCurrentlyWatering() const { return currentlyWatering; }

    // Setters
    void setPlantNodeUUID(int pnUUID) { plantNode_uuid = pnUUID; }
    void setUserUUID(int userUUID) { user_uuid = userUUID; }
    void setNodeName(const string &nName) { nodeName = nName; }
    void setPlantSpecies(const string &pSpecies) { plantSpecies = pSpecies; }
    void setWateringFrequency(const string &wF) { wateringFrequency = wF; }
    void setSoilMoisture(int sM) { soilMoisture = sM; }
    void setLastWatered(const string &lW) { lastWatered = lW; }
    void setLocation(const string &loc) { location = loc; }
    void setCurrentlyWatering(bool cW) { currentlyWatering = cW; }
};

PlantNode pnNotFound(0, 0, "", "", "", 0, "", "");

class PlantNodeManager
{
private:
    vector<PlantNode> plantNodes;

public:
    PlantNodeManager() {}

    int getTotalNodes()
    {
        return plantNodes.size();
    }

    void addPlantNode(PlantNode &pn)
    {
        plantNodes.push_back(pn);
    }

    void removePlantNode(int plantNodeUUID)
    {
        for (int i = 0; i < plantNodes.size(); i++)
        {
            if (plantNodes[i].getPlantNodeUUID() == plantNodeUUID)
            {
                plantNodes.erase(plantNodes.begin() + i);
                break;
            }
        }
    }

    void setPlantNode(int plantNodeUUID, PlantNode &pn)
    {
        for (int i = 0; i < plantNodes.size(); i++)
        {
            if (plantNodes[i].getPlantNodeUUID() == plantNodeUUID)
            {
                plantNodes[i] = pn;
                break;
            }
        }
    }

    PlantNode getPlantNode(int plantNodeUUID)
    {
        for (int i = 0; i < plantNodes.size(); i++)
        {
            if (plantNodes[i].getPlantNodeUUID() == plantNodeUUID)
            {
                return plantNodes[i];
                break;
            }
        }

        return pnNotFound;
    }
};
