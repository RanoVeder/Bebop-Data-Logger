#include "Logger.h"

std::shared_ptr<spdlog::logger> Logger::debugLogger;

void Logger::Init()
{
    debugLogger = spdlog::stdout_color_mt("DEBUG");
}

