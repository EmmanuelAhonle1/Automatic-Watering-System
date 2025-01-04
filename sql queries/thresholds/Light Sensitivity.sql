-- Light Sensitivity Thresholds
CREATE TABLE `threshold-light-sensitivity` (
  `lightThresholdID` int NOT NULL AUTO_INCREMENT,
  `threshold_name` varchar(50) NOT NULL,
  `min_light_level` int NOT NULL,
  `max_light_level` int NOT NULL,
  `description` text,
  PRIMARY KEY (`lightThresholdID`),
  UNIQUE KEY `threshold_name` (`threshold_name`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

INSERT INTO `threshold-light-sensitivity` (threshold_name, min_light_level, max_light_level, description) VALUES
   ('Low Light', 0, 9, 'Suitable for shade-loving plants'),
   ('Medium Light', 10, 25, 'Suitable for partial shade plants'),
   ('Bright Indirect', 26, 49, 'Ideal for most indoor plants'),
   ('Direct Light', 50, 85, 'Suitable for sun-loving plants'),
   ('Full Sun', 86, 100, 'For desert and outdoor plants');