import React from "react";
import { createRoot } from "react-dom/client";
import "./styles.css";
import PlantCardContainer from "./components/Dashboard/CardContainer/CardContainer";
import DashboardHeader from "./components/Dashboard/Header/Header";

const App = () => {
  // Example plant data

  return (
    <div>
      <DashboardHeader />
      <PlantCardContainer />
    </div>
  );
};

// Create root and render
const container = document.getElementById("root");
const root = createRoot(container);
root.render(<App />);

export default App;
