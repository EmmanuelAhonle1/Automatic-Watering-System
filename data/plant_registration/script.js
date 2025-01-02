// Default settings for different plant species
const plantDefaults = {
  monstera: {
    wateringFrequency: "5", // Weekly
    thresholdLight: "3", // Bright Indirect
    thresholdHumidity: "4", // High
    thresholdMoisture: "4", // Moist
    thresholdTemperature: "2", // Moderate
  },
  snake_plant: {
    wateringFrequency: "6", // Biweekly
    thresholdLight: "2", // Medium Light
    thresholdHumidity: "2", // Low
    thresholdMoisture: "2", // Dry
    thresholdTemperature: "2", // Moderate
  },
  peace_lily: {
    wateringFrequency: "5", // Weekly
    thresholdLight: "3", // Bright Indirect
    thresholdHumidity: "4", // High
    thresholdMoisture: "4", // Moist
    thresholdTemperature: "2", // Moderate
  },
  pothos: {
    wateringFrequency: "5", // Weekly
    thresholdLight: "3", // Bright Indirect
    thresholdHumidity: "3", // Medium
    thresholdMoisture: "3", // Medium
    thresholdTemperature: "2", // Moderate
  },
  succulent: {
    wateringFrequency: "6", // Biweekly
    thresholdLight: "5", // Full Sun
    thresholdHumidity: "2", // Low
    thresholdMoisture: "2", // Dry
    thresholdTemperature: "3", // Warm
  },
};

function getCookies(name) {
  const value = `; ${document.cookie}`;
  const parts = value.split(`; ${name}=`);
  if (parts.length === 2) return parts.pop().split(";").shift();
}

// Load saved settings from localStorage
function loadSavedSettings() {
  const savedSettings = localStorage.getItem("plantNodeSettings");
  if (savedSettings) {
    const settings = JSON.parse(savedSettings);
    Object.keys(settings).forEach((key) => {
      const element = document.getElementById(key);
      if (element) {
        element.value = settings[key];
      }
    });
  }
}

function loadOptions(id, endpoint) {
  const select = document.querySelector(id);

  fetch(endpoint)
    .then((response) => response.json())
    .then((data) => {
      const sortedArr = data.sort((a, b) => {
        return a.id - b.id;
      });
      console.log(sortedArr);

      // Populate the select element with sorted options
      sortedArr.forEach((element) => {
        select.add(new Option(element.name, element.id));
      });
    })
    .catch((error) => {
      console.error("There was a problem with the fetch operation:", error);
    });
}

(function loadAllOptions() {
  console.log("hello");
  loadOptions(
    "#wateringFrequency",
    "https://automatic-watering-system-api-e673f34a5955.herokuapp.com/wifiConfig/retrieveWateringFrequencies"
  );
  loadOptions(
    "#thresholdHumidity",
    "https://automatic-watering-system-api-e673f34a5955.herokuapp.com/wifiConfig/retrieveHumidityThresholds"
  );
  loadOptions(
    "#thresholdLight",
    "https://automatic-watering-system-api-e673f34a5955.herokuapp.com/wifiConfig/retrieveLightThresholds"
  );
  loadOptions(
    "#thresholdMoisture",
    "https://automatic-watering-system-api-e673f34a5955.herokuapp.com/wifiConfig/retrieveMoistureThresholds"
  );
  loadOptions(
    "#thresholdTemperature",
    "https://automatic-watering-system-api-e673f34a5955.herokuapp.com/wifiConfig/retrieveTemperatureThresholds"
  );
})();

// Update settings based on plant species
function updateSettings() {
  const species = document.getElementById("plantSpecies").value;
  if (species === "custom") {
    document.getElementById("customSpecies").style.display = "block";
  } else {
    document.getElementById("customSpecies").style.display = "none";
    if (species && plantDefaults[species]) {
      const defaults = plantDefaults[species];
      Object.keys(defaults).forEach((key) => {
        const element = document.getElementById(key);
        if (element) {
          element.value = defaults[key];
        }
      });
    }
  }
}

async function getMacAddress() {
  try {
    const response = await fetch(`/get-MAC`, {
      method: "GET",
      headers: {
        "Content-Type": "application/json",
      },
    });
    const data = await response.json();
    return data.macAddress;
  } catch (error) {
    console.error("Error fetching MAC address:", error);
  }
}

settings = {};
// Save settings
function saveSettings(event) {
  //event.preventDefault();

  settings = {
    nodeName: document.getElementById("nodeName").value,
    plantSpecies:
      document.getElementById("plantSpecies").value === "custom"
        ? document.getElementById("customSpecies").value
        : document.getElementById("plantSpecies").value,
    wateringFrequencyID: document.getElementById("wateringFrequency").value,
    lightThresholdID: document.getElementById("thresholdLight").value,
    humidityThresholdID: document.getElementById("thresholdHumidity").value,
    moistureThresholdID: document.getElementById("thresholdMoisture").value,
    temperatureThresholdID: document.getElementById("thresholdTemperature")
      .value,
  };

  // Validate all fields are filled
  const emptyFields = Object.keys(settings).filter((key) => !settings[key]);
  if (emptyFields.length > 0) {
    alert("Please fill in all fields");
    return false;
  }

  // Save to localStorage
  localStorage.setItem("plantNodeSettings", JSON.stringify(settings));

  // Here you would typically send the settings to your plant node
  console.log("Settings saved:", settings);

  return false;
}

// Reset form
function resetForm() {
  if (confirm("Are you sure you want to reset all settings?")) {
    document.getElementById("settingsForm").reset();
    localStorage.removeItem("plantNodeSettings");
  }
}

// Load saved settings when page loads
document.addEventListener("DOMContentLoaded", loadSavedSettings);

async function submitForm() {
  saveSettings();
  // Here you would typically send the data to a server

  try {
    const macAddress = await getMacAddress();
    settings.macAddress = macAddress;
    console.log("Submitting form:", settings);

    console.log("MAC Address: " + settings.macAddress);

    const response = await fetch(
      `https://automatic-watering-system-api-e673f34a5955.herokuapp.com/plantNode/newPlantNode`,
      {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        credentials: "include",
        body: JSON.stringify({
          nodeName: settings.nodeName,
          plantSpecies: settings.plantSpecies,
          wateringFrequencyID: settings.wateringFrequencyID,
          lightThresholdID: settings.lightThresholdID,
          humidityThresholdID: settings.humidityThresholdID,
          moistureThresholdID: settings.moistureThresholdID,
          temperatureThresholdID: settings.temperatureThresholdID,
          macAddress: settings.macAddress, // Include the macAddress here
        }),
      }
    );

    const data = await response.json();
    if (!data.error) {
      alert("Plant node registered successfully! This tab will now close.");
      self.close();
    } else {
      alert("Error registering plant node: " + data.error);
    }

    // Clear form
    document.getElementById("registrationForm").reset();
    toggleCustomFrequency(); // Hide custom frequency input
    console.log("Form submitted:", data);
  } catch (error) {
    console.error("Error submitting form:", error);
  }
}

async function validateForm(event) {
  event.preventDefault();

  // Reset error messages
  document.getElementById("nodeNameError").textContent = "";
  document.getElementById("plantSpeciesError").textContent = "";
  document.getElementById("wateringFrequencyError").textContent = "";

  // Get form values
  const nodeName = document.getElementById("nodeName").value.trim();
  const plantSpecies = document.getElementById("plantSpecies").value.trim();
  const wateringFrequency = document.getElementById("wateringFrequency").value;
  const customFrequency = document.getElementById("customFrequency").value;
  let isValid = true;

  // Validate node name
  if (nodeName.length < 3) {
    document.getElementById("nodeNameError").textContent =
      "Node name must be at least 3 characters long";
    isValid = false;
  }

  // Validate plant species
  if (plantSpecies.length < 3) {
    document.getElementById("plantSpeciesError").textContent =
      "Plant species must be at least 3 characters long";
    isValid = false;
  }

  // Validate watering frequency
  if (
    wateringFrequency === "custom" &&
    (customFrequency < 1 || customFrequency > 365)
  ) {
    document.getElementById("wateringFrequencyError").textContent =
      "Custom frequency must be between 1 and 365 days";
    isValid = false;
  }

  if (isValid) {
    submitForm();
  }

  return false;
}

function toggleCustomFrequency() {
  const wateringFrequency = document.getElementById("wateringFrequency").value;
  const customFrequencyInput = document.getElementById("customFrequency");
  if (wateringFrequency === "custom") {
    customFrequencyInput.style.display = "block";
  } else {
    customFrequencyInput.style.display = "none";
    customFrequencyInput.value = ""; // Clear custom frequency input
  }
}

// Add event listeners for real-time validation
document.getElementById("nodeName").addEventListener("input", function (e) {
  if (e.target.value.trim().length >= 3) {
    document.getElementById("nodeNameError").textContent = "";
  }
});

document.getElementById("plantSpecies").addEventListener("input", function (e) {
  if (e.target.value.trim().length >= 3) {
    document.getElementById("plantSpeciesError").textContent = "";
  }
});

document
  .getElementById("customFrequency")
  .addEventListener("input", function (e) {
    if (e.target.value >= 1 && e.target.value <= 365) {
      document.getElementById("wateringFrequencyError").textContent = "";
    }
  });

function toggleCustomSpecies() {
  const plantSpecies = document.getElementById("plantSpecies").value;
  const customSpeciesInput = document.getElementById("customSpecies");
  if (plantSpecies === "custom") {
    customSpeciesInput.style.display = "block";
  } else {
    customSpeciesInput.style.display = "none";
    customSpeciesInput.value = ""; // Clear custom species input
  }
}

document
  .getElementById("plantSpecies")
  .addEventListener("change", updateSettings);
