INSERT INTO users (
    userUUID,
    username,
    email,
    password,
    firstName,
    lastName,
    phoneNumber,
    preferredTemperatureUnit,
    notificationsEnabled,
    isActive,
    lastLogin
) VALUES (
    UUID(),
    'johndoe',
    'john.doe@example.com',
    SHA2('SecurePassword123!', 256),
    'John',
    'Doe',
    '+1234567890',
    'F',
    1,
    1,
    NOW()
);
