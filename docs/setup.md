# MySQL Database Setup Instructions

## Overview

This document provides instructions for setting up the MySQL database required for the project.

## Prerequisites

Make sure you have the following installed:

- **MySQL Server**
- **MySQL Workbench** (optional, but recommended)

## Step 1: Create the Database

Run the SQL scripts provided in the `database` directory. Execute them in the following order:

1. `threshold-humidity.sql`
2. `threshold-light-sensitivity.sql`
3. `threshold-soil-moisture.sql`
4. `threshold-temperature.sql`
5. `watering_frequencies.sql`
6. `users.sql`
7. `plant_nodes.sql`
8. `triggers.sql`

These scripts will:

- Create necessary tables.
- Set up constraints and relationships.
- Populate reference data.

## Step 2: Update Configuration

In your firmware directory, locate the `config_template.h` file.

1. **Rename `config_template.h` to `config.h`.**
2. **Update the following placeholders:**

   ```cpp
   #ifndef CONFIG_H
   #define CONFIG_H

   #include <Arduino.h>

   // Define placeholders for sensitive information
   #define SERVER_HOST "your_server_host"
   #define USER "your_user"
   #define PASSWORD "your_password"

   #endif // CONFIG_H
   ```

Replace `your_server_host`, `your_user`, and `your_password` with your database credentials.

## Step 3: Verify Connection

After configuring, test the connection by running the firmware and observing logs.

## Additional Notes

- Ensure that your database user has the necessary permissions.
- Secure your configuration file to prevent unauthorized access.

## Troubleshooting

If you encounter any issues, refer to the error logs or reach out to the project maintainer.

---

For additional details or troubleshooting, check the main README file.
