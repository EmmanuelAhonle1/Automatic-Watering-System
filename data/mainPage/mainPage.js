class Plant {
  numOfPlants = 0;
  constructor(
    name,
    species,
    wateringFrequency,
    optimalMoistureRange,
    lastWatered = null,
    soilMoisture = null,
    sunlightRequirements = null,
    location = null,
    temperature = null,
    humidity = null
  ) {
    this.name = name;
    this.species = species;
    this.wateringFrequency = wateringFrequency; // in days
    this.optimalMoistureRange = optimalMoistureRange; // e.g., [40, 60]

    // Optional parameters, default to null if not provided
    this.lastWatered = lastWatered;
    this.soilMoisture = soilMoisture;
    this.sunlightRequirements = sunlightRequirements;
    this.location = location;
    this.temperature = temperature;
    this.humidity = humidity;
  }

  waterPlant() {
    this.lastWatered = new Date();
    console.log(`${this.name} has been watered.`);
  }

  checkMoistureLevel(currentMoisture) {
    this.soilMoisture = currentMoisture;
    if (currentMoisture < this.optimalMoistureRange[0]) {
      return `${this.name} needs water!`;
    } else if (currentMoisture > this.optimalMoistureRange[1]) {
      return `${this.name} has too much water.`;
    } else {
      return `${this.name} is at an ideal moisture level.`;
    }
  }
}
