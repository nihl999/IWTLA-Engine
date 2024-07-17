#pragma once

#include <core/defines.h>
#include <format>
#include <string>

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#ifdef OURO_RELEASE
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

// kohi engine #3
namespace Ouroboros {
typedef enum log_level {
  LEVEL_FATAL = 0,
  LEVEL_ERROR = 1,
  LEVEL_WARN = 2,
  LEVEL_INFO = 3,
  LEVEL_DEBUG = 4,
  LEVEL_TRACE = 5
} log_level;

// TODO: SHOULDN'T EXPORT
OUROAPI void ShutdownLogging();
OUROAPI bool StartupLogging();

OUROAPI void log_output(log_level level, std::string message);

template <typename... Args>
void log_output(log_level level, std::format_string<Args...> message,
                Args &&...args) {
  std::string msg = std::format(message, std::forward<Args>(args)...);
  return Ouroboros::log_output(level, msg);
};

} // namespace Ouroboros

#ifndef OUROFATAL
#define OUROFATAL(message, ...)                                                \
  Ouroboros::log_output(Ouroboros::LEVEL_FATAL, message, ##__VA_ARGS__);
#endif

#ifndef OUROERROR
#define OUROERROR(message, ...)                                                \
  Ouroboros::log_output(Ouroboros::LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_WARN_ENABLED == 1
#define OUROWARN(message, ...)                                                 \
  Ouroboros::log_output(Ouroboros::LEVEL_WARN, message, ##__VA_ARGS__);
#else
#define OUROWARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
#define OUROINFO(message, ...)                                                 \
  Ouroboros::log_output(Ouroboros::LEVEL_INFO, message, ##__VA_ARGS__);
#else
#define OUROINFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define OURODEBUG(message, ...)                                                \
  Ouroboros::log_output(Ouroboros::LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
#define OURODEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define OUROTRACE(message, ...)                                                \
  Ouroboros::log_output(Ouroboros::LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define OUROTRACE(message, ...)
#endif