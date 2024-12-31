// Default settings for different plant species
const plantDefaults = {
  monstera: {
    wateringFrequency: "3",
    lightThreshold: "2500",
    humidityThreshold: "60",
    moistureThreshold: "40",
    temperatureThreshold: "22",
  },
  snake_plant: {
    wateringFrequency: "14",
    lightThreshold: "1000",
    humidityThreshold: "40",
    moistureThreshold: "20",
    temperatureThreshold: "20",
  },
  peace_lily: {
    wateringFrequency: "7",
    lightThreshold: "1000",
    humidityThreshold: "50",
    moistureThreshold: "40",
    temperatureThreshold: "20",
  },
  pothos: {
    wateringFrequency: "7",
    lightThreshold: "1000",
    humidityThreshold: "50",
    moistureThreshold: "30",
    temperatureThreshold: "20",
  },
  succulent: {
    wateringFrequency: "14",
    lightThreshold: "5000",
    humidityThreshold: "40",
    moistureThreshold: "20",
    temperatureThreshold: "24",
  },
};

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

// Auto-fill settings based on plant species
document
  .getElementById("plantSpecies")
  .addEventListener("change", function (e) {
    const species = e.target.value;
    if (species && plantDefaults[species]) {
      const defaults = plantDefaults[species];
      Object.keys(defaults).forEach((key) => {
        const element = document.getElementById(key);
        if (element) {
          element.value = defaults[key];
        }
      });
    }
  });

// Save settings
function saveSettings(event) {
  event.preventDefault();

  const settings = {
    nodeName: document.getElementById("nodeName").value,
    plantSpecies: document.getElementById("plantSpecies").value,
    wateringFrequency: document.getElementById("wateringFrequency").value,
    lightThreshold: document.getElementById("lightThreshold").value,
    humidityThreshold: document.getElementById("humidityThreshold").value,
    moistureThreshold: document.getElementById("moistureThreshold").value,
    temperatureThreshold: document.getElementById("temperatureThreshold").value,
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
  alert("Settings saved successfully!");

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

function validateForm(event) {
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
    // Here you would typically send the data to a server
    console.log("Form submitted:", {
      nodeName,
      plantSpecies,
      wateringFrequency,
      customFrequency,
    });

    // Simulate registration success
    alert("Plant node registered successfully!");

    // Clear form
    document.getElementById("registrationForm").reset();
    toggleCustomFrequency(); // Hide custom frequency input
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
