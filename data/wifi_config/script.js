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

document.getElementById("wifi-form").onsubmit = function (e) {
  e.preventDefault();
  const formData = new FormData(e.target);
  const status = document.getElementById("status");

  status.innerHTML = "Connecting...";
  status.className = "";

  fetch("/wifi-connect", {
    method: "POST",
    body: formData,
  })
    .then((response) => response.json())
    .then((data) => {
      if (data.success) {
        status.innerHTML = "Connected successfully! Redirecting...";
        status.className = "success";
        setTimeout(() => {
          window.location.href = "/";
        }, 3000);
      } else {
        status.innerHTML = "Connection failed: " + data.message;
        status.className = "error";
      }
    })
    .catch((error) => {
      status.innerHTML = "Error connecting to network";
      status.className = "error";
    });
};

document.addEventListener("DOMContentLoaded", scanNetworks);
