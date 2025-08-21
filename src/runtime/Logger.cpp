#include "runtime/Logger.hpp"
#include <SDL3/SDL_log.h>
#include <chrono>
#include <print>

uint32_t Logger::_maxCategory = (uint32_t)SDL_LOG_CATEGORY_CUSTOM;
std::unordered_map<int, std::string> Logger::_categoryNames = {
    {SDL_LOG_CATEGORY_APPLICATION, "Application"},
    {SDL_LOG_CATEGORY_ERROR, "Error"},
    {SDL_LOG_CATEGORY_ASSERT, "Assert"},
    {SDL_LOG_CATEGORY_SYSTEM, "System"},
    {SDL_LOG_CATEGORY_AUDIO, "Audio"},
    {SDL_LOG_CATEGORY_VIDEO, "Video"},
    {SDL_LOG_CATEGORY_RENDER, "Render"},
    {SDL_LOG_CATEGORY_INPUT, "Input"},
    {SDL_LOG_CATEGORY_TEST, "Test"},
    {SDL_LOG_CATEGORY_GPU, "GPU"},
};
std::unordered_map<std::string, std::unique_ptr<Logger>> Logger::_loggers;
void Logger::print(void *userdata, int category, SDL_LogPriority priority,
                   const char *message) {
  std::string kind;
  switch (priority) {
  case SDL_LOG_PRIORITY_INVALID:
    kind = "Invalid";
    break;
  case SDL_LOG_PRIORITY_TRACE:
    kind = "Trace";
    break;
  case SDL_LOG_PRIORITY_VERBOSE:
    kind = "Verbose";
    break;
  case SDL_LOG_PRIORITY_DEBUG:
    kind = "Debug";
    break;
  case SDL_LOG_PRIORITY_INFO:
    kind = "Info";
    break;
  case SDL_LOG_PRIORITY_WARN:
    kind = "Warn";
    break;
  case SDL_LOG_PRIORITY_ERROR:
    kind = "Error";
    break;
  case SDL_LOG_PRIORITY_CRITICAL:
    kind = "Critical";
    break;
  case SDL_LOG_PRIORITY_COUNT:
    kind = "Count";
    break;
  default:
    kind = "Unknown";
    break;
  }
  std::string categoryName = "Unkown";

  if (_categoryNames.contains(category)) {
    categoryName = _categoryNames.at(category);
  }
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm *now_tm = std::localtime(&now_time);
  std::string dateTime =
      std::format("{:04}/{:02}/{:02} {:02}:{:02}:{:02}", now_tm->tm_year + 1900,
                  now_tm->tm_mon + 1, now_tm->tm_mday, now_tm->tm_hour,
                  now_tm->tm_min, now_tm->tm_sec);
  std::println("[{}] [{}] [{}]: {}", dateTime, categoryName, kind, message);
}
Logger::Logger(uint32_t category) : _category(category) {}
void Logger::setPriorities(const std::string &logPriorities) {
  if (logPriorities == "trace") {
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_TRACE);
  } else if (logPriorities == "verbose") {
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
  } else if (logPriorities == "debug") {
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);
  } else if (logPriorities == "info") {
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_INFO);
  } else if (logPriorities == "warn") {
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_WARN);
  } else if (logPriorities == "error") {
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_ERROR);
  } else if (logPriorities == "critical") {
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_CRITICAL);
  } else {
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Unknown log priority: %s",
                logPriorities.c_str());
  }
}
Logger *Logger::getLogger(const std::string &category) {
  auto &plogger = _loggers[category];
  if (!plogger) {
    plogger.reset(new Logger(_maxCategory));
    _categoryNames[_maxCategory] = category;
    _maxCategory++;
  }
  return plogger.get();
}