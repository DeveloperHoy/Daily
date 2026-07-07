#pragma once
#include <string>

namespace ai::core {

enum class ApplicationState {
    STATE_IDLE = 0,
    STATE_INITIALIZING,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_PAUSING,
    STATE_STOPPING
    STATE_ERROR,
    STATE_SHUTDOWN
};

inline bool isTerminalState(ApplicationState state) {
    return state == ApplicationState::STATE_STOPPED || 
           state == ApplicationState::STATE_ERROR || 
           state == ApplicationState::STATE_SHUTDOWN;
}

inline bool isRunnableState(ApplicationState state) {
    return state == ApplicationState::STATE_RUNNING || 
           state == ApplicationState::STATE_PAUSED;
}

inline std::string toString(ApplicationState state) {
    switch (state) {
        case ApplicationState::STATE_IDLE: return "IDLE";
        case ApplicationState::STATE_INITIALIZING: return "INITIALIZING";
        case ApplicationState::STATE_RUNNING: return "RUNNING";
        case ApplicationState::STATE_PAUSED: return "PAUSED";
        case ApplicationState::STATE_PAUSING: return "PAUSING";
        case ApplicationState::STATE_STOPPED: return "STOPPED";
        case ApplicationState::STATE_ERROR: return "ERROR";
        case ApplicationState::STATE_SHUTDOWN: return "SHUTDOWN";
        default: return "UNKNOWN";
    }
}

} // namespace ai::core
