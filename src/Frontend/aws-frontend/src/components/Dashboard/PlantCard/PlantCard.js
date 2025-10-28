import React from "react";
import "./PlantCard.css";

const PlantCard = ({ plantData }) => {
  return (
    <div className="card">
      <h2>{plantData.name}</h2>
      <p>Species: {plantData.species}</p>
      <p>Watering Frequency: {plantData.wateringFrequency}</p>
      <p>Soil Moisture: {plantData.soilMoisture}</p>
      <p>Last Watered: {plantData.lastWatered}</p>
      <p>Location: {plantData.location}</p>
      <p>Sunlight Requirements: {plantData.sunlightRequirements}</p>
      <p>Temperature: {plantData.temperature}</p>
      <p>Humidity: {plantData.humidity}</p>
    </div>
  );
};

export default PlantCard;
