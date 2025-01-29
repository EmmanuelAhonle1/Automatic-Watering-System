import { useState, useEffect, useCallback, use } from "react";
import "./CardContainer.css";
import { plants } from "./testPlants";
import PlantCard from "../PlantCard/PlantCard";
//const CardContainer = ({ children }) => {

const PlantCardContainer = () => {
  const [plants, setPlants] = useState([]);

  useEffect(() => {
    // Fetch plant data

    const fetchedPlants = plants;
    setPlants(fetchedPlants);
  }, []);

  return (
    <div className="card-container">
      {plants.map((plant) => (
        <PlantCard id={plant.id} plantData={plant} />
      ))}
    </div>
  );
};
