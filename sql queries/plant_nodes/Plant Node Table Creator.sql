CREATE TABLE IF NOT EXISTS plant_nodes (
    plantNodeUUID CHAR(39),      
    nodeName VARCHAR(100) NOT NULL UNIQUE,
    connectedUserUUID CHAR(36) UNIQUE,
    plantSpecies VARCHAR(100),
    wateringFrequencyID INT NOT NULL,
    lightThresholdID INT NOT NULL,
    humidityThresholdID INT NOT NULL,
    moistureThresholdID INT NOT NULL,
    temperatureThresholdID INT NOT NULL,
    soilMoisture FLOAT NOT NULL,
    CONSTRAINT valid_moisture CHECK (soilMoisture >= 0 AND soilMoisture <= 100),
    lastWatered TIMESTAMP NOT NULL,
    location VARCHAR(100),
    needsWatering BOOLEAN DEFAULT FALSE,
    lastInitialWaterTrigger TIMESTAMP NULL,    -- Added: tracks when watering started
    maxWateringTime INT DEFAULT 30,            -- Added: in seconds, defaults to 30
    
    -- Environmental data
    temperature FLOAT,          
    humidity FLOAT,             
    lightLevel FLOAT,           
    
    -- System status
    healthStatus VARCHAR(20) DEFAULT 'Healthy',    
    batteryLevel FLOAT,         
    isOnline BOOLEAN DEFAULT TRUE,           
    lastSync TIMESTAMP,         
    
    -- Maintenance tracking
    nextWateringTime TIMESTAMP,  
    lastMaintenance TIMESTAMP,   
    notes TEXT,                 
    macAddress CHAR(17) NOT NULL,              -- Added MAC address field
    CONSTRAINT valid_mac CHECK (macAddress REGEXP '^[0-9A-Fa-f]{2}(:[0-9A-Fa-f]{2}){5}$'), -- Ensure MAC address format
    
    -- Creation/Update tracking
    createdAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updatedAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    
    PRIMARY KEY (plantNodeUUID),
    FOREIGN KEY (wateringFrequencyID) REFERENCES watering_frequencies(frequencyID),
    FOREIGN KEY (lightThresholdID) REFERENCES `threshold-light-sensitivity`(lightThresholdID),
    FOREIGN KEY (humidityThresholdID) REFERENCES `threshold-humidity`(humidityThresholdID),
    FOREIGN KEY (moistureThresholdID) REFERENCES `threshold-soil-moisture`(moistureThresholdID),
    FOREIGN KEY (temperatureThresholdID) REFERENCES `threshold-temperature`(temperatureThresholdID)
);

-- Updated watering status trigger
DELIMITER //
CREATE TRIGGER update_watering_status
BEFORE UPDATE ON plant_nodes
FOR EACH ROW
BEGIN
    DECLARE min_moisture_threshold FLOAT;
    
    -- Get the minimum moisture threshold
    SELECT min_moisture INTO min_moisture_threshold
    FROM `threshold-soil-moisture`
    WHERE moistureThresholdID = NEW.moistureThresholdID;
    
    -- If currently not watering and moisture is below threshold
    IF (OLD.needsWatering = FALSE AND NEW.soilMoisture < min_moisture_threshold) THEN
        SET NEW.needsWatering = TRUE;
        SET NEW.lastInitialWaterTrigger = CURRENT_TIMESTAMP;
    -- If currently watering, check if we should stop
    ELSEIF (OLD.needsWatering = TRUE) THEN
        -- Stop if moisture is 15 above threshold or max time elapsed
        IF (NEW.soilMoisture > (min_moisture_threshold + 15) OR 
            TIMESTAMPDIFF(SECOND, NEW.lastInitialWaterTrigger, CURRENT_TIMESTAMP) > NEW.maxWateringTime) THEN
            SET NEW.needsWatering = FALSE;
            SET NEW.lastInitialWaterTrigger = NULL;
        END IF;
    END IF;
END;//
DELIMITER ;


DELIMITER //
CREATE TRIGGER before_insert_plant_nodes
   BEFORE INSERT ON plant_nodes
   FOR EACH ROW
BEGIN
   SET NEW.plantNodeUUID = CONCAT('pn-', UUID());
END;//
DELIMITER ;


-- Health Status Trigger
DELIMITER //
CREATE TRIGGER update_plant_health
BEFORE UPDATE ON plant_nodes
FOR EACH ROW
BEGIN
   IF (NEW.lightLevel < (SELECT min_light_level FROM `threshold-light-sensitivity` WHERE lightThresholdID = NEW.lightThresholdID)
       OR NEW.lightLevel > (SELECT max_light_level FROM `threshold-light-sensitivity` WHERE lightThresholdID = NEW.lightThresholdID)
       OR NEW.humidity < (SELECT min_humidity FROM `threshold-humidity` WHERE humidityThresholdID = NEW.humidityThresholdID)
       OR NEW.humidity > (SELECT max_humidity FROM `threshold-humidity` WHERE humidityThresholdID = NEW.humidityThresholdID)
       OR NEW.soilMoisture < (SELECT min_moisture FROM `threshold-soil-moisture` WHERE moistureThresholdID = NEW.moistureThresholdID)
       OR NEW.soilMoisture > (SELECT max_moisture FROM `threshold-soil-moisture` WHERE moistureThresholdID = NEW.moistureThresholdID)
       OR NEW.temperature < (SELECT min_temperature FROM `threshold-temperature` WHERE temperatureThresholdID = NEW.temperatureThresholdID)
       OR NEW.temperature > (SELECT max_temperature FROM `threshold-temperature` WHERE temperatureThresholdID = NEW.temperatureThresholdID)) THEN
       SET NEW.healthStatus = 'Needs Attention';
   ELSE
       SET NEW.healthStatus = 'Healthy';
   END IF;
END;//
DELIMITER ;