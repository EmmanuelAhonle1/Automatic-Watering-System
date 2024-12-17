-- Light Sensitivity Thresholds
CREATE TABLE IF NOT EXISTS light_sensitivity_thresholds (
   lightThresholdID INT AUTO_INCREMENT,
   threshold_name VARCHAR(50) NOT NULL UNIQUE,
   min_light_level INT NOT NULL,
   max_light_level INT NOT NULL,
   description TEXT,
   PRIMARY KEY (lightThresholdID)
);

INSERT INTO light_sensitivity_thresholds (threshold_name, min_light_level, max_light_level, description) VALUES
   ('Low Light', 0, 9, 'Suitable for shade-loving plants'),
   ('Medium Light', 10, 25, 'Suitable for partial shade plants'),
   ('Bright Indirect', 26, 49, 'Ideal for most indoor plants'),
   ('Direct Light', 50, 85, 'Suitable for sun-loving plants'),
   ('Full Sun', 86, 100, 'For desert and outdoor plants');