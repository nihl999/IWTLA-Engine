#include <core/logger.h>
#include <memory>
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// internal
std::shared_ptr<spdlog::logger> engineLogger;
// std::shared_ptr<spdlog::logger> applicationLogger;

void Ouroboros::ShutdownLogging() {}

bool Ouroboros::StartupLogging() {
  spdlog::set_pattern("%^[%T] %n: %v%$");
  engineLogger = spdlog::stdout_color_mt("Ouroboros");
  //   applicationLogger = spdlog::stdout_color_mt("Application");

  //   applicationLogger->set_level(spdlog::level::trace);
  engineLogger->set_level(spdlog::level::trace);

  return true;
}

void Ouroboros::log_output(log_level level, std::string message) {
  switch (level) {
  case LEVEL_FATAL:
    engineLogger->critical(message);
    break;
  case LEVEL_ERROR:
    engineLogger->error(message);
    break;
  case LEVEL_WARN:
    engineLogger->warn(message);
    break;
  case LEVEL_INFO:
    engineLogger->info(message);
    break;
  case LEVEL_DEBUG:
    engineLogger->debug(message);
    break;
  case LEVEL_TRACE:
    engineLogger->trace(message);
    break;
  }
}
