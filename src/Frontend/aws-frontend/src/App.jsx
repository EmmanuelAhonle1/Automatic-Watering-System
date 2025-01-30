import React from "react";
import "./styles.css";
import PlantCardContainer from "./components/Dashboard/CardContainer/CardContainer";
import DashboardHeader from "./components/Dashboard/Header/Header";
// import Sidebar, { SidebarItem } from "sidebar/src/components/Sidebar";

const App = () => {
  // Example plant data

  return (
    <div>
      <DashboardHeader />
      <PlantCardContainer />
    </div>
  );
};

export default App;
