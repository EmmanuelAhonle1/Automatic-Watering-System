DELETE FROM plant_nodes 
WHERE plantNodeUUID = 'pn-specific-uuid-here'
AND macAddress REGEXP '^[0-9A-Fa-f]{2}(:[0-9A-Fa-f]{2}){5}$';  -- Ensure MAC address format