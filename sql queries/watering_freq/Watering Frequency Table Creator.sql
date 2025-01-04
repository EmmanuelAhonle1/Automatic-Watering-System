CREATE TABLE `watering_frequencies` (
  `wateringFrequencyID` int NOT NULL,
  `frequency_name` varchar(50) NOT NULL,
  `hours_between_watering` int NOT NULL,
  `description` text,
  PRIMARY KEY (`wateringFrequencyID`),
  UNIQUE KEY `frequency_name` (`frequency_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Example insert statements for common frequencies:
INSERT INTO watering_frequencies (frequency_name, hours_between_watering, description) VALUES
    ('Twice Daily', 12, 'Waters every 12 hours'),
    ('Daily', 24, 'Waters once every day'),
    ('Every Other Day', 48, 'Waters every 48 hours'),
    ('Twice Weekly', 84, 'Waters about twice per week'),
    ('Weekly', 168, 'Waters once per week'),
    ('Biweekly', 336, 'Waters every two weeks'),
    ('Monthly', 720, 'Waters once per month'),
    ('Custom', NULL, 'Custom watering schedule');