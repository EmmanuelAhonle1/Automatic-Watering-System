
CREATE TRIGGER before_insert_users 
BEFORE INSERT ON users 
FOR EACH ROW 
BEGIN
    SET NEW.userUUID = CONCAT('usr-', UUID());
END;

CREATE TRIGGER phone_sanitize_trigger 
BEFORE INSERT ON users 
FOR EACH ROW 
BEGIN
    SET NEW.phoneNumber = REPLACE(
        REPLACE(
            REPLACE(
                REPLACE(NEW.phoneNumber, '-', ''),
                '(', ''
            ),
            ')', ''
        ),
        ' ', ''
    );
END;

CREATE TRIGGER before_insert_plant_nodes 
BEFORE INSERT ON plant_nodes 
FOR EACH ROW 
BEGIN
    SET NEW.plantNodeUUID = CONCAT('pn-', UUID());
END;


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
END;



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
END;


CREATE TRIGGER set_last_watered_time 
BEFORE INSERT ON plant_nodes 
FOR EACH ROW 
BEGIN
    SET NEW.lastWatered = NOW();
END;