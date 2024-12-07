

enum PlantNodeState
{
    STATE_SETUP,        // Initial state on boot, before WiFi is connected
    STATE_AP_CONFIG,    // ESP is in AP mode, waiting for user to connect and configure
    STATE_CONNECTING,   // Connecting to WiFi
    STATE_CONNECTED,    // Connected to WiFi
    STATE_RECONNECTING, // Lost connection, attempting to connect
};

const char *getStateName(PlantNodeState state)
{
    switch (state)
    {
    case STATE_SETUP:
        return "Setup";
    case STATE_AP_CONFIG:
        return "Access Point Configuration";
    case STATE_CONNECTING:
        return "Connecting";
    case STATE_CONNECTED:
        return "Connected";
    case STATE_RECONNECTING:
        return "Reconnecting";
    default:
        return "UNKNOWN";
    }
}