const hashPassword = async (password) => {
  return password;
};

document.addEventListener("DOMContentLoaded", function () {
  fetch(
    "https://automatic-watering-system-api-e673f34a5955.herokuapp.com/users/login",
    {
      method: "GET",
      credentials: "include",
      headers: {
        "Content-Type": "application/json",
      },
    }
  )
    .then((response) => {
      console.log("Response status:", response.status); // Add debug logging
      if (!response.ok) {
        if (response.status === 401) {
          console.log("Not logged in - redirecting to login page");
          return null;
        }
        throw new Error("Network response was not ok");
      }
      return response.json();
    })
    .then((data) => {
      if (data && data.username) {
        console.log("Login data:", data); // Add debug logging
        window.location.href = `http://${window.location.hostname}/plant_registration/index.html`;
      }
    })
    .catch((error) => {
      console.error("Error:", error);
    });
});

function validateForm(event) {
  event.preventDefault();

  // Reset error messages
  document.getElementById("usernameError").textContent = "";
  document.getElementById("passwordError").textContent = "";

  // Get form values
  const username = document.getElementById("username").value.trim();
  const password = document.getElementById("password").value;
  const rememberMe = document.getElementById("remember").checked;
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
    hashPassword(password)
      .then((hashedPassword) => {
        console.log("Attempting login...");
        return fetch(
          "https://automatic-watering-system-api-e673f34a5955.herokuapp.com/users/login",
          {
            method: "POST",
            credentials: "include",
            headers: {
              "Content-Type": "application/json",
            },
            body: JSON.stringify({
              username,
              password: hashedPassword,
              rememberMe,
            }),
          }
        );
      })
      .then((response) => {
        if (!response.ok) {
          return response.json().then((err) => Promise.reject(err));
        }
        return response.json();
      })
      .then((data) => {
        if (data.success) {
          console.log("Login successful!");
          localStorage.setItem("isLoggedIn", "true");
          window.location.href = `http://${window.location.hostname}/plant_registration/index.html`;
        } else {
          alert(data.error || "Login failed. Please try again.");
        }
      })
      .catch((error) => {
        console.error("Error during login:", error);
        alert(
          error.error || "An error occurred during login. Please try again."
        );
      });
  }

  return false;
}

function validateSignupForm(event) {
  event.preventDefault();

  // Reset error messages
  document.getElementById("signupUsernameError").textContent = "";
  document.getElementById("signupEmailError").textContent = "";
  document.getElementById("signupPasswordError").textContent = "";
  document.getElementById("firstNameError").textContent = "";
  document.getElementById("lastNameError").textContent = "";
  document.getElementById("phoneNumberError").textContent = "";
  document.getElementById("temperatureUnitError").textContent = "";

  // Get form values
  const username = document.getElementById("signupUsername").value.trim();
  const email = document.getElementById("signupEmail").value.trim();
  const password = document.getElementById("signupPassword").value;
  const firstName = document.getElementById("firstName").value.trim();
  const lastName = document.getElementById("lastName").value.trim();
  const phoneNumber = document.getElementById("phoneNumber").value.trim();
  const temperatureUnit = document.getElementById("temperatureUnit").value;
  let isValid = true;

  // Validate username
  if (username.length < 3) {
    document.getElementById("signupUsernameError").textContent =
      "Username must be at least 3 characters long";
    isValid = false;
  }

  // Validate email
  if (!email.includes("@")) {
    document.getElementById("signupEmailError").textContent =
      "Invalid email address";
    isValid = false;
  }

  // Validate password
  if (password.length < 6) {
    document.getElementById("signupPasswordError").textContent =
      "Password must be at least 6 characters long";
    isValid = false;
  }

  // Validate first name
  if (firstName.length < 1) {
    document.getElementById("firstNameError").textContent =
      "First name is required";
    isValid = false;
  }

  // Validate last name
  if (lastName.length < 1) {
    document.getElementById("lastNameError").textContent =
      "Last name is required";
    isValid = false;
  }

  // Validate phone number
  if (phoneNumber.length < 10) {
    document.getElementById("phoneNumberError").textContent =
      "Phone number must be at least 10 digits long";
    isValid = false;
  }

  if (isValid) {
    hashPassword(password)
      .then((hashedPassword) => {
        console.log("Hashed password:", hashedPassword);
        return fetch(
          `https://automatic-watering-system-api-e673f34a5955.herokuapp.com/users/signup`,
          {
            method: "POST",
            headers: {
              "Content-Type": "application/json",
              "Access-Control-Allow-Origin": "*",
              "Access-Control-Allow-Methods": "GET, POST, OPTIONS",
              "Access-Control-Allow-Headers": "Content-Type",
            },
            body: JSON.stringify({
              username,
              email,
              password: hashedPassword,
              firstName,
              lastName,
              phoneNumber,
              preferredTemperatureUnit: temperatureUnit,
            }),
          }
        );
      })
      .then((response) => {
        if (response.ok) {
          alert("Signup successful! Please log in.");
          toggleForms(); // Switch back to login form
        } else {
          alert("Signup failed. Please try again.");
        }
      })
      .catch((error) => {
        console.error("Error during signup:", error);
      });
  }

  return false;
}

function toggleForms() {
  const loginForm = document.getElementById("loginForm");
  const signupForm = document.getElementById("signupForm");
  if (loginForm.style.display === "none") {
    loginForm.style.display = "block";
    signupForm.style.display = "none";
  } else {
    loginForm.style.display = "none";
    signupForm.style.display = "block";
  }
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

document
  .getElementById("signupUsername")
  .addEventListener("input", function (e) {
    if (e.target.value.trim().length >= 3) {
      document.getElementById("signupUsernameError").textContent = "";
    }
  });

document.getElementById("signupEmail").addEventListener("input", function (e) {
  if (e.target.value.includes("@")) {
    document.getElementById("signupEmailError").textContent = "";
  }
});

document
  .getElementById("signupPassword")
  .addEventListener("input", function (e) {
    if (e.target.value.length >= 6) {
      document.getElementById("signupPasswordError").textContent = "";
    }
  });

document.getElementById("firstName").addEventListener("input", function (e) {
  if (e.target.value.trim().length >= 1) {
    document.getElementById("firstNameError").textContent = "";
  }
});

document.getElementById("lastName").addEventListener("input", function (e) {
  if (e.target.value.trim().length >= 1) {
    document.getElementById("lastNameError").textContent = "";
  }
});

document.getElementById("phoneNumber").addEventListener("input", function (e) {
  if (e.target.value.trim().length >= 10) {
    document.getElementById("phoneNumberError").textContent = "";
  }
});

function logout() {
  fetch(
    "https://automatic-watering-system-api-e673f34a5955.herokuapp.com/users/logout",
    {
      method: "POST",
      credentials: "include",
      headers: {
        "Content-Type": "application/json",
      },
    }
  ).then((response) => {
    if (response.ok) {
      localStorage.removeItem("isLoggedIn");
      window.location.href = "/intro_login/index.html";
    }
  });
}
