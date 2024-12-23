#ifndef PLANTNODE_HPP
#define PLANTNODE_HPP

#define DEBUG_PLANT_NODE
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
    /**
     * @brief Construct a new Plant Node object
     *
     * @param pnUUID Plant node unique identifier
     * @param userUUID User unique identifier
     * @param nName Node name
     * @param pSpecies Plant species
     * @param wF Watering frequency
     * @param sM Soil moisture
     * @param lW Last watered date
     * @param loc Location
     */
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
    /**
     * @brief Construct a new Plant Node Manager object
     */
    PlantNodeManager();

    /**
     * @brief Get the total number of plant nodes
     *
     * @return int Total number of plant nodes
     */
    int getTotalNodes();

    /**
     * @brief Add a new plant node
     *
     * @param pn PlantNode object to add
     */
    void addPlantNode(PlantNode &pn);

    /**
     * @brief Remove a plant node by its unique identifier
     *
     * @param plantNodeUUID Unique identifier of the plant node to remove
     */
    void removePlantNode(int plantNodeUUID);

    /**
     * @brief Set the details of a plant node by its unique identifier
     *
     * @param plantNodeUUID Unique identifier of the plant node to set
     * @param pn PlantNode object with updated details
     */
    void setPlantNode(int plantNodeUUID, PlantNode &pn);

    /**
     * @brief Get the details of a plant node by its unique identifier
     *
     * @param plantNodeUUID Unique identifier of the plant node to get
     * @return PlantNode PlantNode object with the details
     */
    PlantNode getPlantNode(int plantNodeUUID);
};

#endif // PLANTNODE_HPP
