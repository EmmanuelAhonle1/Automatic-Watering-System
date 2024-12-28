CREATE TABLE IF NOT EXISTS users (
    userUUID CHAR(36),
    username VARCHAR(50) NOT NULL UNIQUE,
    email VARCHAR(100) NOT NULL UNIQUE,
    passwordHash VARCHAR(255) NOT NULL,   -- For storing hashed passwords
    firstName VARCHAR(50),
    lastName VARCHAR(50),
    phoneNumber VARCHAR(15),
    
    -- User preferences
    preferredTemperatureUnit ENUM('C', 'F') DEFAULT 'F',
    notificationsEnabled BOOLEAN DEFAULT TRUE,
    
    -- Account status
    isActive BOOLEAN DEFAULT TRUE,
    lastLogin TIMESTAMP,
    
    -- Creation/Update tracking
    createdAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updatedAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    
    PRIMARY KEY (userUUID)
);watering_frequencieswatering_frequencies

DELIMITER //
CREATE TRIGGER before_insert_users
    BEFORE INSERT ON users
    FOR EACH ROW
BEGIN
    SET NEW.userUUID = CONCAT('usr-', UUID());
END;//
DELIMITER ;