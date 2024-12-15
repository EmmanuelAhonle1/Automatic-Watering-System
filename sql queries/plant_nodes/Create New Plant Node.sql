-- Updated INSERT with thresholds
INSERT INTO plant_nodes (
    nodeName,
    connectedUserUUID,
    plantSpecies,
    wateringFrequencyID,
    lightThresholdID,      -- Added
    humidityThresholdID,   -- Added
    moistureThresholdID,   -- Added
    soilMoisture,
    lastWatered,
    location,
    temperature,
    humidity,
    lightLevel,
    healthStatus,
    batteryLevel,
    isOnline,
    lastSync,
    nextWateringTime
) VALUES (
    'Plant-Node-0001',
    'usr-uuid-goes-here',
    'Monstera Deliciosa',
    1,
    3,                     -- Bright Indirect Light (801-1500)
    4,                     -- High Humidity (71-85%)
    4,                     -- Moist Soil (61-80%)
    75.5,
    CURRENT_TIMESTAMP,
    'Living Room Window',
    23.5,
    65.0,
    800.0,
    'Healthy',
    98.5,
    TRUE,
    CURRENT_TIMESTAMP,
    DATE_ADD(CURRENT_TIMESTAMP, INTERVAL 24 HOUR)
);

-- Logic to update healthStatus based on thresholds
UPDATE plant_nodes pn
SET healthStatus = 
    CASE 
        WHEN pn.lightLevel < (SELECT min_light_level FROM light_sensitivity_thresholds lst WHERE lst.lightThresholdID = pn.lightThresholdID)
        OR pn.lightLevel > (SELECT max_light_level FROM light_sensitivity_thresholds lst WHERE lst.lightThresholdID = pn.lightThresholdID)
        OR pn.humidity < (SELECT min_humidity FROM humidity_thresholds ht WHERE ht.humidityThresholdID = pn.humidityThresholdID)
        OR pn.humidity > (SELECT max_humidity FROM humidity_thresholds ht WHERE ht.humidityThresholdID = pn.humidityThresholdID)
        OR pn.soilMoisture < (SELECT min_moisture FROM soil_moisture_thresholds smt WHERE smt.moistureThresholdID = pn.moistureThresholdID)
        OR pn.soilMoisture > (SELECT max_moisture FROM soil_moisture_thresholds smt WHERE smt.moistureThresholdID = pn.moistureThresholdID)
        THEN 'Needs Attention'
        ELSE 'Healthy'
    END
WHERE plantNodeUUID = 'specific-plant-node-uuid';