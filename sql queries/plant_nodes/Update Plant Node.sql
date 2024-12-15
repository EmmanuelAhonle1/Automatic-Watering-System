UPDATE plant_nodes 
SET 
   nodeName = 'New-Node-Name',             
   plantSpecies = 'New Plant Species',
   wateringFrequencyID = 2,                
   lightThresholdID = 3,                   -- Bright Indirect (801-1500)
   humidityThresholdID = 3,                -- Medium (51-70%)
   moistureThresholdID = 4,                -- Moist (61-80%)
   soilMoisture = 82.5,                    
   location = 'New Location',
   temperature = 24.5,                     
   humidity = 70.0,                        
   lightLevel = 850.0,
   batteryLevel = 75.5,                    
   isOnline = TRUE,
   lastSync = CURRENT_TIMESTAMP,
   nextWateringTime = DATE_ADD(CURRENT_TIMESTAMP, INTERVAL 48 HOUR),
   notes = 'Plant showing new growth, adjusted watering schedule'
WHERE plantNodeUUID = 'pn-specific-uuid-here';