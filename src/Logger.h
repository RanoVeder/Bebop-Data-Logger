#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"

class Logger
{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger> getDebugLogger() { return debugLogger; };

	private:
		static std::shared_ptr<spdlog::logger> debugLogger;
};


#define DEBUG_DEBUG(...) ::Logger::getDebugLogger()->debug(__VA_ARGS__)
#define DEBUG_TRACE(...) ::Logger::getDebugLogger()->trace(__VA_ARGS__)
#define DEBUG_INFO(...) ::Logger::getDebugLogger()->info(__VA_ARGS__)
#define DEBUG_WARN(...) ::Logger::getDebugLogger()->warn(__VA_ARGS__)
#define DEBUG_ERROR(...) ::Logger::getDebugLogger()->error(__VA_ARGS__)
