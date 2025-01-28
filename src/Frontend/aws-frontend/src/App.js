import React from "react";
import { createRoot } from "react-dom/client";
import "./styles.css";

import PlantCard from "./components/Dashboard/PlantCard/PlantCard";
import DashboardHeader from "./components/Dashboard/Header/Header";

const App = () => {
  const plantData = {
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

  return (
    <div>
      <DashboardHeader />
      <div id="card-container" className="card-container">
        <PlantCard plant={plantData} />
        <PlantCard plant={plantData} />
        <PlantCard plant={plantData} />
      </div>
    </div>
  );
};

// Create root and render
const container = document.getElementById("root");
const root = createRoot(container);
root.render(<App />);

export default App;
