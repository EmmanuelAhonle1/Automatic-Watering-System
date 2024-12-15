INSERT INTO users (
    username,
    email,
    passwordHash,
    firstName,
    lastName,
    phoneNumber,
    -- preferredTemperatureUnit,
    notificationsEnabled
) VALUES (
    'john_doef',
    'john.doey@email.com',
    '$2y$10$abcdefghijklmnopqrstuv',  -- This would be an actual hashed password
    'John',
    'Doe',
    '+1-555-123-4567',
    -- 'C',
    TRUE
);