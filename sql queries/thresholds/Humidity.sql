-- Humidity Thresholds
CREATE TABLE `threshold-humidity` (
  `humidityThresholdID` int NOT NULL AUTO_INCREMENT,
  `threshold_name` varchar(50) NOT NULL,
  `min_humidity` int NOT NULL,
  `max_humidity` int NOT NULL,
  `description` text,
  PRIMARY KEY (`humidityThresholdID`),
  UNIQUE KEY `threshold_name` (`threshold_name`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

INSERT INTO `threshold-humidity` (threshold_name, min_humidity, max_humidity, description) VALUES
   ('Very Low', 0, 30, 'Suitable for cacti and succulents'),
   ('Low', 31, 50, 'Standard indoor humidity'),
   ('Medium', 51, 70, 'Ideal for most houseplants'),
   ('High', 71, 85, 'Tropical plants'),
   ('Very High', 86, 100, 'Orchids and ferns');
