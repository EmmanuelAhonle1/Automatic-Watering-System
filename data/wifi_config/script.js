function scanNetworks() {
  const select = document.getElementById("network");
  select.innerHTML = '<option value="">Scanning...</option>';

  fetch("/wifi-scan")
    .then((response) => response.json())
    .then((networks) => {
      select.innerHTML = '<option value="">Select a network...</option>';
      networks.sort((a, b) => b.rssi - a.rssi); // Sort by signal strength
      networks.forEach((network) => {
        const option = document.createElement("option");
        option.value = network.ssid;
        const signalStrength =
          network.rssi > -50
            ? "Excellent"
            : network.rssi > -60
              ? "Good"
              : network.rssi > -70
                ? "Fair"
                : "Poor";
        option.textContent = `${network.ssid} (${signalStrength}: ${network.rssi}dBm)`;
        select.appendChild(option);
      });
    })
    .catch((error) => {
      console.error("Error scanning networks:", error);
      select.innerHTML = '<option value="">Error scanning networks</option>';
    });
}

document.getElementById("wifi-form").onsubmit = async function (e) {
  e.preventDefault();
  const formData = new FormData(e.target);
  const status = document.getElementById("status");
  const form = e.target;

  // Disable form while connecting
  const inputs = form.querySelectorAll("input, select, button");
  inputs.forEach((input) => (input.disabled = true));

  try {
    status.innerHTML = "Connecting to WiFi...";
    status.className = "info";

    // First attempt to connect to WiFi
    const connectResponse = await fetch("/wifi-connect", {
      method: "POST",
      body: formData,
    });

    if (!connectResponse.ok) {
      throw new Error(`HTTP error! status: ${connectResponse.status}`);
    }

    const data = await connectResponse.json();

    if (data.success) {
      status.innerHTML = "WiFi connected! Closing access point...";
      status.className = "success";

      try {
        // Then disable the access point
        const disableResponse = await fetch("/disable-ap", {
          method: "POST",
          headers: {
            "Cache-Control": "no-cache",
            Pragma: "no-cache",
          },
        });

        if (!disableResponse.ok) {
          throw new Error(`Failed to disable AP: ${disableResponse.status}`);
        }

        status.innerHTML =
          "Connected successfully! Redirecting to login for device registration...";

        // Redirect to the new device IP
        if (data.ip) {
          setTimeout(() => {
            window.location.href = `http://${data.ip}/intro_login/index.html`;
          }, 6000);
        } else {
          throw new Error("No IP address received from device");
        }
      } catch (apError) {
        console.error("Error disabling AP:", apError);
        status.innerHTML =
          "Connected to WiFi but failed to disable AP. Please refresh the page.";
        status.className = "warning";
      }
    } else {
      status.innerHTML = `Connection failed: ${data.message}`;
      status.className = "error";
      // Re-enable form on failure
      inputs.forEach((input) => (input.disabled = false));
    }
  } catch (error) {
    console.error("Connection error:", error);
    status.innerHTML = "Error connecting to network. Please try again.";
    status.className = "error";
    // Re-enable form on error
    inputs.forEach((input) => (input.disabled = false));
  }
};

// Add a rescan button handler
document.getElementById("rescan-button")?.addEventListener("click", (e) => {
  e.preventDefault();
  scanNetworks();
});

// Initial network scan
document.addEventListener("DOMContentLoaded", scanNetworks);
