#include "runtime/Application.hpp"
#include "SDL3/SDL_render.h"
#include "render/RenderSystem.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>
#include <chrono>
#include <print>
#include <string>

static void LogOutput(void *userdata, int category, SDL_LogPriority priority,
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
  std::string categoryName;
  switch (category) {
  case SDL_LOG_CATEGORY_APPLICATION:
    categoryName = "Application";
    break;
  case SDL_LOG_CATEGORY_ERROR:
    categoryName = "Error";
    break;
  case SDL_LOG_CATEGORY_ASSERT:
    categoryName = "Assert";
    break;
  case SDL_LOG_CATEGORY_SYSTEM:
    categoryName = "System";
    break;
  case SDL_LOG_CATEGORY_AUDIO:
    categoryName = "Audio";
    break;
  case SDL_LOG_CATEGORY_VIDEO:
    categoryName = "Video";
    break;
  case SDL_LOG_CATEGORY_RENDER:
    categoryName = "Render";
    break;
  case SDL_LOG_CATEGORY_INPUT:
    categoryName = "Input";
    break;
  case SDL_LOG_CATEGORY_TEST:
    categoryName = "Test";
    break;
  case SDL_LOG_CATEGORY_GPU:
    categoryName = "GPU";
    break;
  default:
    categoryName = "Unknown";
    break;
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
std::unique_ptr<Application> Application::_instance = nullptr;

void Application::resolveOptions(int argc, char **argv) {
  _cwd = SDL_GetBasePath();
  if (_cwd.empty()) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not get base path: %s",
                 SDL_GetError());
    return;
  }
  int index = 1;
  while (index < argc) {
    std::string arg = argv[index];
    if (arg.starts_with("--")) {
      // Long option
      auto pos = arg.find('=');
      if (pos != std::string::npos) {
        std::string key = arg.substr(2, pos - 2);
        std::string value = arg.substr(pos + 1);
        _options[key] = value;
      } else {
        _options[arg.substr(2)] = "true"; // Flag option
      }
    } else if (arg.starts_with('-')) {
      // Short option
      for (size_t i = 1; i < arg.size(); ++i) {
        char opt = arg[i];
        if (i + 1 < arg.size() && arg[i + 1] == '=') {
          _options[std::string(1, opt)] = arg.substr(i + 2);
          break;
        } else {
          _options[std::string(1, opt)] = "true"; // Flag option
        }
      }
    }
    index++;
  }
}
void Application::initLog() {
  SDL_SetLogOutputFunction(LogOutput, nullptr);
  auto &logPriorities = getOption("log_priority", "info");
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
bool Application::createWindowAndRenderer() {
  _window =
      SDL_CreateWindow("Thaumic Industrial", 1024, 768, SDL_WINDOW_RESIZABLE);
  if (!_window) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s",
                 SDL_GetError());
    return false;
  }
  _renderer = SDL_CreateRenderer(_window, nullptr);
  if (!_renderer) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create renderer: %s",
                 SDL_GetError());
    return false;
  }
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Window created successfully");
  return true;
}

void Application::cleanup() {

  RendererSystem::getInstance().clearLayers();
  if (_renderer) {
    SDL_DestroyRenderer(_renderer);
    _renderer = nullptr;
  }
  if (_window) {
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Application cleaned up");
  SDL_Quit();
}
bool Application::processEvent() {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
      SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
                   "Window close requested, exiting loop");
      _running = false;
      return true;
    default:
      break;
    }
  }
  return false;
}

void Application::onUpdate() {
  if (!processEvent()) {
    SDL_RenderClear(_renderer);
    RendererSystem::getInstance().draw(_renderer);
    SDL_RenderPresent(_renderer);
  }
}

const std::string &Application::getOption(const std::string &key,
                                          const std::string &def) const {
  auto it = _options.find(key);
  if (it != _options.end()) {
    return it->second;
  }
  return def;
}

Application::~Application() { cleanup(); }

void Application::exit() {
  _running = false;
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Exiting application");
}

int Application::run(int argc, char **argv) {
  resolveOptions(argc, argv);
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Could not initialize SDL: %s", SDL_GetError());
    return -1;
  }
  initLog();
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Application running with CWD: %s",
              _cwd.c_str());
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
               "Application started with options:");
  for (auto &[option, value] : _options) {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Option: %s = %s",
                 option.c_str(), value.c_str());
  }
  if (!createWindowAndRenderer()) {
    return -1;
  }
  while (_running) {
    onUpdate();
  }
  return 0;
}