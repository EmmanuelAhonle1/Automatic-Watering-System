import { useState, useEffect, useCallback } from "react";
import "./CardContainer.css";
//import { owned_plants } from "./testPlants";
import PlantCard from "../PlantCard/PlantCard";
//const CardContainer = ({ children }) => {
const fern = {
  id: "pn-9092093490243",
  name: "Fern",
  species: "Adiantum",
  wateringFrequency: "Every 3 days",
  soilMoisture: "45%",
  lastWatered: "2023-10-01",
  location: "Living Room",
  sunlightRequirements: "Partial Shade",
  temperature: "20°C",
  humidity: "70%",
};

const cactus = {
  id: "pn-32340",
  name: "Cactus",
  species: "Echinocactus",
  wateringFrequency: "Every 2 weeks",
  soilMoisture: "20%",
  lastWatered: "2023-09-25",
  location: "Office",
  sunlightRequirements: "Full Sun",
  temperature: "25°C",
  humidity: "30%",
};

const orchid = {
  id: "pn-523325209",
  name: "Orchid",
  species: "Phalaenopsis",
  wateringFrequency: "Once a week",
  soilMoisture: "60%",
  lastWatered: "2023-09-28",
  location: "Bedroom",
  sunlightRequirements: "Indirect Light",
  temperature: "22°C",
  humidity: "50%",
};

export const plantArr = [fern, orchid, cactus];

const PlantCardContainer = () => {
  const [owned_plants, setOwnedPlants] = useState([]);

  useEffect(() => {
    // TODO: replace with API call
    // TODO: sanitize fetched data
    const fetchedPlants = plantArr;
    console.log("Retrieved plants");
    setOwnedPlants(fetchedPlants);
  }, []);

  return (
    <div className="card-container">
      {owned_plants.length > 0 ? (
        owned_plants.map((plant) => (
          <PlantCard key={plant.id} plantData={plant} />
        ))
      ) : (
        <p>"Loading..."</p>
      )}
    </div>
  );
};

export default PlantCardContainer;
