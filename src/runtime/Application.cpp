#include "runtime/Application.hpp"
#include "core/ScopeGuard.hpp"
#include "render/RenderSystem.hpp"
#include "runtime/AssetManager.hpp"
#include "runtime/ImageLoader.hpp"
#include "runtime/Locale.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <chrono>
#include <exception>
#include <memory>
#include <print>
#include <string>
#include <unordered_map>

void Application::logOutput(void *userdata, int category,
                            SDL_LogPriority priority, const char *message) {
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

  auto app = Application::getInstance();
  if (app->_logCategoryNames.contains(category)) {
    categoryName = app->_logCategoryNames.at(category);
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
  SDL_SetLogOutputFunction(logOutput, nullptr);
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
  _logCategoryNames[SDL_LOG_CATEGORY_APPLICATION] = "Application";
  _logCategoryNames[SDL_LOG_CATEGORY_ERROR] = "Error";
  _logCategoryNames[SDL_LOG_CATEGORY_ASSERT] = "Assert";
  _logCategoryNames[SDL_LOG_CATEGORY_SYSTEM] = "System";
  _logCategoryNames[SDL_LOG_CATEGORY_AUDIO] = "Audio";
  _logCategoryNames[SDL_LOG_CATEGORY_VIDEO] = "Video";
  _logCategoryNames[SDL_LOG_CATEGORY_RENDER] = "Render";
  _logCategoryNames[SDL_LOG_CATEGORY_INPUT] = "Input";
  _logCategoryNames[SDL_LOG_CATEGORY_TEST] = "Test";
  _logCategoryNames[SDL_LOG_CATEGORY_GPU] = "GPU";
}
bool Application::createWindow() {
  _window =
      SDL_CreateWindow("Thaumic Industrial", 1024, 768, SDL_WINDOW_RESIZABLE);
  if (!_window) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s",
                 SDL_GetError());
    return false;
  }
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Window created successfully");
  return true;
}
bool Application::initAssetManager() {
  try {
    auto imgLoader = std::make_shared<ImageLoader>();
    _assetManager = new AssetManager();
    _assetManager->registerLoader("png", imgLoader);
    _assetManager->registerLoader("bmp", imgLoader);
    _assetManager->registerLoader("jpeg", imgLoader);
    _assetManager->registerLoader("jpg", imgLoader);
    return true;
  } catch (std::exception &e) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to create asset manager: %s", e.what());
  } catch (...) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to create asset manager: unknown exception");
  }
  return false;
}

bool Application::initRenderSystem() {
  try {
    SDL_Renderer *renderer = SDL_CreateRenderer(_window, NULL);
    if (!renderer) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                   "Failed to create renderer: %s", SDL_GetError());
      return false;
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER,
                 "Create renderer successful with device: %s",
                 SDL_GetRendererName(renderer));
    _renderSystem = new RenderSystem(renderer);
    return true;
  } catch (std::exception &e) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to create render system: %s", e.what());
  } catch (...) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to create render system: unknown exception");
  }
  return false;
}
bool Application::initLocale() {
  try {
    _locale = new Locale();
    _locale->addLanguage("en_US", "English (US)");
    _locale->addLanguage("zh_CN", "简体中文");
    _locale->setDefaultLang("en_US");
    _locale->setLang("en_US");
    return true;
  } catch (std::exception &e) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create locale: %s",
                 e.what());
  } catch (...) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Failed to create locale: unknown exception");
  }
  return false;
}

void Application::cleanup() {
  if (_locale) {
    delete _locale;
    _locale = nullptr;
  }
  if (_assetManager) {
    delete _assetManager;
    _assetManager = nullptr;
  }
  if (_renderSystem) {
    delete _renderSystem;
    _renderSystem = nullptr;
  }
  if (_window) {
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }
  TTF_Quit();
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Application cleaned up");
  SDL_Quit();
}
void Application::onWindowClose() {
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
               "Window close requested, exiting loop");
  _running = false;
}
bool Application::processEvent() {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
      onWindowClose();
      break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
      break;
    default:
      break;
    }
    return true;
  }
  return false;
}
void Application::onPreInitialize() {}
void Application::onInitialize() {
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Support languages:");
  for (auto &[key, name] : _locale->getLanguages()) {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "  key: %s, name: %s",
                 key.c_str(), name.c_str());
  }
}
void Application::onPostInitialize() {}
void Application::onUpdate() {
  if (!processEvent()) {
    _renderSystem->present();
  }
}
void Application::onUninitialize() {}

const std::string &Application::getOption(const std::string &key,
                                          const std::string &def) const {
  auto it = _options.find(key);
  if (it != _options.end()) {
    return it->second;
  }
  return def;
}

Application::~Application() {}

void Application::exit() {
  _running = false;
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Exiting application");
}

int Application::run(int argc, char **argv) {
  DEFER([this] { cleanup(); });
  resolveOptions(argc, argv);
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Could not initialize SDL: %s", SDL_GetError());
    return -1;
  }
  initLog();
  if (!TTF_Init()) {
    SDL_Log("Could not initialize TTF: %s", SDL_GetError());
    return -1;
  }
  _cwd = SDL_GetBasePath();
  if (_cwd.empty()) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not get base path: %s",
                 SDL_GetError());
    return -1;
  }
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Application running with CWD: %s",
              _cwd.c_str());
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
               "Application started with options:");
  for (auto &[option, value] : _options) {
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Option: %s = %s",
                 option.c_str(), value.c_str());
  }
  if (!initAssetManager()) {
    return -1;
  }
  if (!createWindow()) {
    return -1;
  }
  if (!initRenderSystem()) {
    return -1;
  }
  if (!_assetManager->initStore(_cwd + "assets")) {
    SDL_LogError(SDL_LOG_CATEGORY_SYSTEM,
                 "Failed to load asset store: %sassets", _cwd.c_str());
  }
  if (!initLocale()) {
    return -1;
  }
  onPreInitialize();
  onInitialize();
  onPostInitialize();
  while (_running) {
    onUpdate();
  }
  onUninitialize();
  return 0;
}