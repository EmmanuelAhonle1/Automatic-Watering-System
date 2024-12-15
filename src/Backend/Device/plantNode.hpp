#ifndef PLANTNODE_HPP
#define PLANTNODE_HPP

#include <string>
#include <vector>

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
    bool currentlyWatering;
    string location;

public:
    PlantNode(int pnUUID, int userUUID, string nName, string pSpecies, string wF, int sM, string lW, string loc);

    // Getters
    int getPlantNodeUUID() const;
    int getUserUUID() const;
    string getNodeName() const;
    string getPlantSpecies() const;
    string getWateringFrequency() const;
    int getSoilMoisture() const;
    string getLastWatered() const;
    string getLocation() const;
    bool isCurrentlyWatering() const;

    // Setters
    void setPlantNodeUUID(int pnUUID);
    void setUserUUID(int userUUID);
    void setNodeName(const string &nName);
    void setPlantSpecies(const string &pSpecies);
    void setWateringFrequency(const string &wF);
    void setSoilMoisture(int sM);
    void setLastWatered(const string &lW);
    void setLocation(const string &loc);
    void setCurrentlyWatering(bool cW);
};

class PlantNodeManager
{
private:
    vector<PlantNode> plantNodes;

public:
    PlantNodeManager();

    int getTotalNodes();
    void addPlantNode(PlantNode &pn);
    void removePlantNode(int plantNodeUUID);
    void setPlantNode(int plantNodeUUID, PlantNode &pn);
    PlantNode getPlantNode(int plantNodeUUID);
};

#endif // PLANTNODE_HPP
