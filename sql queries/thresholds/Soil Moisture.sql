-- Soil Moisture Thresholds
CREATE TABLE `threshold-soil-moisture` (
  `moistureThresholdID` int NOT NULL AUTO_INCREMENT,
  `threshold_name` varchar(50) NOT NULL,
  `min_moisture` int NOT NULL,
  `max_moisture` int NOT NULL,
  `description` text,
  PRIMARY KEY (`moistureThresholdID`),
  UNIQUE KEY `threshold_name` (`threshold_name`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

INSERT INTO `threshold-soil-moisture` (threshold_name, min_moisture, max_moisture, description) VALUES
   ('Very Dry', 0, 20, 'Drought-tolerant plants'),
   ('Dry', 21, 40, 'Succulents and cacti'),
   ('Medium', 41, 60, 'Most indoor plants'),
   ('Moist', 61, 80, 'Tropical plants'),
   ('Wet', 81, 100, 'Aquatic and bog plants');