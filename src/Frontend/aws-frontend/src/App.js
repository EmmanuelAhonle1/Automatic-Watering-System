import React from "react";
import { createRoot } from "react-dom/client";
import "./styles.css";
import { plantArr } from "./components/Dashboard/CardContainer/CardContainer";
import PlantCard from "./components/Dashboard/PlantCard/PlantCard";
import DashboardHeader from "./components/Dashboard/Header/Header";

const App = () => {
  // Example plant data

  return (
    <div>
      <DashboardHeader />
      <div id="card-container" className="card-container">
        <PlantCard plantData={plantArr[0]} />
        <PlantCard plantData={plantArr[1]} />
        <PlantCard plantData={plantArr[2]} />
      </div>
    </div>
  );
};

// Create root and render
const container = document.getElementById("root");
const root = createRoot(container);
root.render(<App />);

export default App;
