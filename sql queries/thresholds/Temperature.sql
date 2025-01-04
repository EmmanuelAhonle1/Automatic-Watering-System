CREATE TABLE `threshold-temperature` (
  `temperatureThresholdID` int NOT NULL AUTO_INCREMENT,
  `threshold_name` varchar(50) NOT NULL,
  `min_temperature` float NOT NULL,
  `max_temperature` float NOT NULL,
  `description` text,
  PRIMARY KEY (`temperatureThresholdID`),
  UNIQUE KEY `threshold_name` (`threshold_name`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

INSERT INTO `threshold-temperature` (threshold_name, min_temperature, max_temperature, description) VALUES
   ('Cool', 10.0, 18.0, 'Suitable for plants that prefer cooler conditions'),
   ('Moderate', 18.1, 24.0, 'Ideal for most indoor plants'),
   ('Warm', 24.1, 29.0, 'Tropical and heat-loving plants'),
   ('Hot', 29.1, 35.0, 'Desert and extreme heat tolerant plants');