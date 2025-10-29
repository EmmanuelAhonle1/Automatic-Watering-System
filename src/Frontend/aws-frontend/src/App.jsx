import React from "react";
import "./styles.css";
import PlantCardContainer from "./components/Dashboard/CardContainer/CardContainer";
import DashboardHeader from "./components/Dashboard/Header/Header";
import Sidebar from "./components/Sidebar/Sidebar.jsx";

const App = () => {
  return (
    <div>
      <DashboardHeader />
      <PlantCardContainer />
      <Sidebar />
    </div>
  );
};

export default App;
