import React from "react";
import "./styles.css";
import PlantCardContainer from "./components/Dashboard/CardContainer/CardContainer";
import DashboardHeader from "./components/Dashboard/Header/Header";
// Temporarily disabled Sidebar import. To re-enable, uncomment the line below.
// import Sidebar from "./components/Sidebar/Sidebar";

const App = () => {
  return (
    <div>
      <DashboardHeader />
      <PlantCardContainer />
      {/* Sidebar temporarily disabled. To re-enable, uncomment the import above and this component:
      <Sidebar />
    */}
    </div>
  );
};

export default App;
