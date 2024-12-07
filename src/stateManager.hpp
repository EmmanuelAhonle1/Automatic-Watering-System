
#include "stateManager.cpp";

extern PlantNodeState currentState;

const char *getStateName(PlantNodeState state);

bool canTransitionTo(PlantNodeState currentState, PlantNodeState newState);