CREATE TABLE IF NOT EXISTS temperature_thresholds (
   temperatureThresholdID INT AUTO_INCREMENT,
   threshold_name VARCHAR(50) NOT NULL UNIQUE,
   min_temperature FLOAT NOT NULL,
   max_temperature FLOAT NOT NULL,
   description TEXT,
   PRIMARY KEY (temperatureThresholdID)
);

INSERT INTO temperature_thresholds (threshold_name, min_temperature, max_temperature, description) VALUES
   ('Cool', 10.0, 18.0, 'Suitable for plants that prefer cooler conditions'),
   ('Moderate', 18.1, 24.0, 'Ideal for most indoor plants'),
   ('Warm', 24.1, 29.0, 'Tropical and heat-loving plants'),
   ('Hot', 29.1, 35.0, 'Desert and extreme heat tolerant plants');