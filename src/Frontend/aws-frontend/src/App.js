import React from "react";
import { createRoot } from "react-dom/client";
import "./styles.css";
import { plants } from "./components/Dashboard/PlantCard/testPlants";
import PlantCard from "./components/Dashboard/PlantCard/PlantCard";
import DashboardHeader from "./components/Dashboard/Header/Header";

const App = () => {
  // Example plant data

  return (
    <div>
      <DashboardHeader />
      <div id="card-container" className="card-container">
        <PlantCard plantData={plants[0]} />
        <PlantCard plantData={plants[1]} />
        <PlantCard plantData={plants[2]} />
      </div>
    </div>
  );
};

// Create root and render
const container = document.getElementById("root");
const root = createRoot(container);
root.render(<App />);

export default App;
