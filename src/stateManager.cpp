

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

// Valid state transition rules
bool canTransitionTo(PlantNodeState currentState, PlantNodeState newState)
{
    switch (currentState)
    {
    case STATE_SETUP:
        return (newState == STATE_AP_CONFIG ||
                newState == STATE_CONNECTING);

    case STATE_AP_CONFIG:
        return (newState == STATE_CONNECTING);

    case STATE_CONNECTING:
        return (newState == STATE_CONNECTED ||
                newState == STATE_AP_CONFIG);

    case STATE_CONNECTED:
        return (newState == STATE_RECONNECTING);

    case STATE_RECONNECTING:
        return (newState == STATE_CONNECTED ||
                newState == STATE_AP_CONFIG);

    default:
        return false;
    }
}