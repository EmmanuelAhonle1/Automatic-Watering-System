# Automatic Watering System

## Overview
The Automatic Watering System is a smart IoT solution for plant care, built using ESP8266 microcontrollers. The system provides automated plant monitoring and watering capabilities with a web-based interface for configuration and monitoring.

## Hardware Components

- ESP8266 Microcontroller
- DHT11 Temperature and Humidity Sensor (Pin D5)
- Capacitive Soil Moisture Sensor (Pin D6)
- VEML7700 Ambient Light Sensor (Pin D7)
- LC709203F Battery Level Sensor (Pin D4)
- RGB Status LED (Pins D0, D1, D2)
- All components are defined in `Pinouts.hpp`

## Features

### 1. WiFi Configuration and Management

- Captive portal for initial WiFi setup
- Automatic reconnection handling
- OTA (Over-the-Air) update support
- Access Point mode with DNS server for initial configuration
- Support for multiple network security types

### 2. Device Management

- Unique device naming system
- Persistent storage of device settings
- MAC address tracking
- JSON-based configuration storage

### 3. Sensor Integration

- Temperature and humidity monitoring
- Soil moisture level detection
- Ambient light measurement
- Battery level monitoring
- All sensor readings available through unified interface

### 4. Status Indication

RGB LED status indicators for:

- WiFi Connected (Blinking Blue)
- AP Mode (Solid Red)
- MySQL Server Connected (Solid Green)
- Connection Lost (Fast Red Blinking)
- Reconnecting (Purple Blinking)

### 5. Web Interface

Three main interfaces:

1. WiFi Configuration (`/wifi_config/`)
   - Network scanning
   - Connection management
   - Status display

2. User Authentication (`/intro_login/`)
   - User login
   - New user registration
   - Session management

3. Plant Registration (`/plant_registration/`)
   - Plant node configuration
   - Species selection
   - Customizable care parameters

## Software Architecture

### Core Components

1. **WiFi Manager** (`wifi_manager.hpp`, `wifi_manager.cpp`)
   - Handles all network connectivity
   - Manages access point and station modes
   - Implements captive portal
   - Handles OTA updates

2. **Device Manager** (`device_manager.hpp`, `device_manager.cpp`)
   - Manages device identification
   - Handles configuration storage
   - Coordinates settings updates

3. **Data Acquisition** (`dataAcquisition.hpp`, `dataAcquisition.cpp`)
   - Interfaces with all sensors
   - Provides unified data reading interface
   - Handles sensor calibration and thresholds

4. **Database Communication** (`DatabaseCommands.hpp`, `DatabaseCommands.cpp`)
   - Manages API interactions
   - Handles data synchronization
   - Implements connection monitoring

5. **RGB Status Handler** (`rgb_status.hpp`, `rgb_status.cpp`)
   - Manages visual feedback
   - Implements state machine for status indication
   - Handles LED control

### Web Interface

The system provides three main web interfaces:

1. **WiFi Configuration**
   - Network scanning and selection
   - Password input
   - Connection status display

2. **User Authentication**
   - Login interface
   - Registration form
   - Session management

3. **Plant Registration**
   - Plant species selection
   - Watering frequency configuration
   - Environmental threshold settings

## Setup and Configuration

### Initial Setup

1. Flash the ESP8266 with the firmware
2. Power on the device
3. Connect to the device's AP (Format: "PlantNode-XXXX")
4. Navigate to the captive portal
5. Configure WiFi settings

### Plant Registration

1. Login or create an account
2. Configure plant parameters:
   - Node name
   - Plant species
   - Watering frequency
   - Environmental thresholds
3. Save configuration

## API Integration

The system integrates with a REST API for:

- User authentication
- Plant node registration
- Settings synchronization
- Data logging

API endpoints are defined in `dbQueryCreator.hpp` and configured via `config.h`

## Development

### Requirements

- Arduino IDE or PlatformIO
- ESP8266 board support
- Required Libraries:
  - ESP8266WiFi
  - ESP8266WebServer
  - ArduinoJson
  - DHT sensor library
  - VEML7700 library
  - LC709203F library
  - RGBLed library

### Building

1. Clone the repository
2. Install required libraries
3. Configure `config.h` with API credentials
4. Build and flash to ESP8266

## Security Features

- Secure WiFi configuration
- Password protection
- Session management
- API authentication
- OTA update support

## Troubleshooting

### LED Status Indicators

- Solid Red: AP Mode (Initial setup required)
- Blinking Blue: Connected to WiFi
- Solid Green: Fully connected (WiFi + Database)
- Fast Red Blinking: Connection lost
- Purple Blinking: Attempting to reconnect

### Common Issues

1. Connection Failures
   - Check WiFi credentials
   - Verify API availability
   - Monitor LED status

2. Sensor Readings
   - Verify sensor connections
   - Check pin configurations
   - Validate sensor thresholds

## Contributing

1. Fork the repository
2. Create a feature branch
3. Submit a pull request

## License


## Authors

Emmanuel Ahonle
