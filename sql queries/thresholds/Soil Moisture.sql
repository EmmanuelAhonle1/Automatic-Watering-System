-- Soil Moisture Thresholds
CREATE TABLE IF NOT EXISTS `threshold-soil-moisture` (
   moistureThresholdID INT AUTO_INCREMENT,
   threshold_name VARCHAR(50) NOT NULL UNIQUE,
   min_moisture INT NOT NULL,
   max_moisture INT NOT NULL,
   description TEXT,
   PRIMARY KEY (moistureThresholdID)
);

INSERT INTO `threshold-soil-moisture` (threshold_name, min_moisture, max_moisture, description) VALUES
   ('Very Dry', 0, 20, 'Drought-tolerant plants'),
   ('Dry', 21, 40, 'Succulents and cacti'),
   ('Medium', 41, 60, 'Most indoor plants'),
   ('Moist', 61, 80, 'Tropical plants'),
   ('Wet', 81, 100, 'Aquatic and bog plants');