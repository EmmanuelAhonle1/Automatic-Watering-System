UPDATE plant_nodes
SET 
    soilMoisture = 42.0,
    needsWatering = TRUE,
    lastWatered = NOW()
WHERE 
    plantNodeUUID = 'pn-1234abcd-5678-efgh-ijkl-9012mnopqrst';
