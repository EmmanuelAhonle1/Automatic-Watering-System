-- Update just email and phone
UPDATE users 
SET 
    email = 'new.email@example.com',
    phoneNumber = '+1-555-999-8888'
WHERE userUUID = 'usr-specific-uuid-here';

-- Update preferences
UPDATE users 
SET 
    preferredTemperatureUnit = 'F',
    notificationsEnabled = TRUE
WHERE userUUID = 'usr-specific-uuid-here';