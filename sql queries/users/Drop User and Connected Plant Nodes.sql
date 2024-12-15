START TRANSACTION;
    DELETE FROM plant_nodes 
    WHERE connectedUserUUID = 'usr-specific-uuid-here';
    
    DELETE FROM users 
    WHERE userUUID = 'usr-specific-uuid-here';
COMMIT;