function validateForm(event) {
  event.preventDefault();

  // Reset error messages
  document.getElementById("usernameError").textContent = "";
  document.getElementById("passwordError").textContent = "";

  // Get form values
  const username = document.getElementById("username").value.trim();
  const password = document.getElementById("password").value;
  let isValid = true;

  // Validate username
  if (username.length < 3) {
    document.getElementById("usernameError").textContent =
      "Username must be at least 3 characters long";
    isValid = false;
  }

  // Validate password
  if (password.length < 6) {
    document.getElementById("passwordError").textContent =
      "Password must be at least 6 characters long";
    isValid = false;
  }

  if (isValid) {
    // Here you would typically send the data to a server
    console.log("Form submitted:", { username, password });

    // Simulate login success
    alert("Login successful!");

    // Clear form
    document.getElementById("loginForm").reset();
  }

  return false;
}

// Add event listeners for real-time validation
document.getElementById("username").addEventListener("input", function (e) {
  if (e.target.value.trim().length >= 3) {
    document.getElementById("usernameError").textContent = "";
  }
});

document.getElementById("password").addEventListener("input", function (e) {
  if (e.target.value.length >= 6) {
    document.getElementById("passwordError").textContent = "";
  }
});
