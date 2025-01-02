CREATE TABLE IF NOT EXISTS watering_frequencies (
    wateringFrequencyID INT AUTO_INCREMENT,
    frequency_name VARCHAR(50) NOT NULL UNIQUE,
    hours_between_watering INT NOT NULL,
    description TEXT,
    PRIMARY KEY (wateringFrequencyID)
);

-- Example insert statements for common frequencies:
INSERT INTO watering_frequencies (frequency_name, hours_between_watering, description) VALUES
    ('Twice Daily', 12, 'Waters every 12 hours'),
    ('Daily', 24, 'Waters once every day'),
    ('Every Other Day', 48, 'Waters every 48 hours'),
    ('Twice Weekly', 84, 'Waters about twice per week'),
    ('Weekly', 168, 'Waters once per week'),
    ('Biweekly', 336, 'Waters every two weeks'),
    ('Monthly', 720, 'Waters once per month'),
    ('Custom', NULL, 'Custom watering schedule');