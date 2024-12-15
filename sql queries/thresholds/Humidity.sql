-- Humidity Thresholds
CREATE TABLE IF NOT EXISTS humidity_thresholds (
   humidityThresholdID INT AUTO_INCREMENT,
   threshold_name VARCHAR(50) NOT NULL UNIQUE,
   min_humidity INT NOT NULL,
   max_humidity INT NOT NULL,
   description TEXT,
   PRIMARY KEY (humidityThresholdID)
);

INSERT INTO humidity_thresholds (threshold_name, min_humidity, max_humidity, description) VALUES
   ('Very Low', 0, 30, 'Suitable for cacti and succulents'),
   ('Low', 31, 50, 'Standard indoor humidity'),
   ('Medium', 51, 70, 'Ideal for most houseplants'),
   ('High', 71, 85, 'Tropical plants'),
   ('Very High', 86, 100, 'Orchids and ferns');
