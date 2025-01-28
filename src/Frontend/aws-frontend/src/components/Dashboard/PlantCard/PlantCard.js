import React from "react";
import "./PlantCard.css";

const PlantCard = ({ plant }) => {
  return (
    <div className="card">
      <h2>{plant.name}</h2>
      <p>Species: {plant.species}</p>
      <p>Watering Frequency: {plant.wateringFrequency}</p>
      <p>Soil Moisture: {plant.soilMoisture}</p>
      <p>Last Watered: {plant.lastWatered}</p>
      <p>Location: {plant.location}</p>
      <p>Sunlight Requirements: {plant.sunlightRequirements}</p>
      <p>Temperature: {plant.temperature}</p>
      <p>Humidity: {plant.humidity}</p>
    </div>
  );
};

export default PlantCard;
