#include <iostream>
#include "application/Application.h"
#include "utils/Logger.h"
#include <cstdlib>

using namespace ai::utils;
using namespace ai::application;

int main(int argc, char* argv[]) {
    Logger logger;
    
    std::string logPath = "logs/app.log";
    if (argc > 2) {
        logPath = argv[2];
    }

    try {
        // Parse args (e.g., config file path)
        // We will pass a dummy config for now
        
        Application app;

        // Start Logging
        logger.setLevel(LogLevel::INFO);

        app.run();

        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        Logger::getInstance().log(LogLevel::ERROR, "Unhandled Exception: " + std::string(e.what()));
        return EXIT_FAILURE;
    }
}
