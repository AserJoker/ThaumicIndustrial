#pragma once
#include "core/Object.hpp"
#include <SDL3/SDL_log.h>
#include <cstdint>
#include <format>
#include <memory>
#include <string>
#include <unordered_map>

class Logger : public Object {
private:
  static uint32_t _maxCategory;
  static std::unordered_map<int, std::string> _categoryNames;
  static std::unordered_map<std::string, std::unique_ptr<Logger>> _loggers;

public:
  static void print(void *userdata, int category, SDL_LogPriority priority,
                    const char *message);
  static void setPriorities(const std::string &priorities);

private:
  uint32_t _category = 0;
  Logger(uint32_t category);

public:
  template <class... Args>
  void trace(std::format_string<Args...> fmt, Args &&...args) {
    SDL_LogTrace(_category, "%s",
                 std::format(fmt, std::forward<Args>(args)...).c_str());
  }
  template <class... Args>
  void debug(std::format_string<Args...> fmt, Args &&...args) {
    SDL_LogDebug(_category, "%s",
                 std::format(fmt, std::forward<Args>(args)...).c_str());
  }
  template <class... Args>
  void verbos(std::format_string<Args...> fmt, Args &&...args) {
    SDL_LogVerbose(_category, "%s",
                   std::format(fmt, std::forward<Args>(args)...).c_str());
  }
  template <class... Args>
  void info(std::format_string<Args...> fmt, Args &&...args) {
    SDL_LogInfo(_category, "%s",
                std::format(fmt, std::forward<Args>(args)...).c_str());
  }
  template <class... Args>
  void warn(std::format_string<Args...> fmt, Args &&...args) {
    SDL_LogWarn(_category, "%s",
                std::format(fmt, std::forward<Args>(args)...).c_str());
  }
  template <class... Args>
  void error(std::format_string<Args...> fmt, Args &&...args) {
    SDL_LogError(_category, "%s",
                 std::format(fmt, std::forward<Args>(args)...).c_str());
  }
  template <class... Args>
  void critial(std::format_string<Args...> fmt, Args &&...args) {
    SDL_LogCritical(_category, "%s",
                    std::format(fmt, std::forward<Args>(args)...).c_str());
  }

public:
  static Logger *getLogger(const std::string &category);
};