#include "runtime/Application.hpp"
#include "core/ScopeGuard.hpp"
#include "render/RenderSystem.hpp"
#include "runtime/AssetManager.hpp"
#include "runtime/ImageLoader.hpp"
#include "runtime/Locale.hpp"
#include "runtime/Logger.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <exception>
#include <memory>
#include <string>
#include <unordered_map>

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
  SDL_SetLogOutputFunction(Logger::print, nullptr);
  auto &logPriorities = getOption("log_priority", "info");
  Logger::setPriorities(logPriorities);
}

bool Application::createWindow() {
  _window =
      SDL_CreateWindow("Thaumic Industrial", 1024, 768, SDL_WINDOW_RESIZABLE);
  if (!_window) {
    _logger->error("Could not create window: {}", SDL_GetError());
    return false;
  }
  _logger->debug("Window created successfully", SDL_GetError());
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
    _logger->error("Failed to create asset manager: {}", SDL_GetError());
  } catch (...) {
    _logger->error("Failed to create asset manager: unknown exception");
  }
  return false;
}

bool Application::initRenderSystem() {
  try {
    SDL_Renderer *renderer = SDL_CreateRenderer(_window, NULL);
    if (!renderer) {
      _logger->error("Failed to create renderer: {}", SDL_GetError());
      return false;
    }
    _logger->debug("Create renderer successful with device: {}",
                  SDL_GetRendererName(renderer));
    _renderSystem = new RenderSystem(renderer);
    return true;
  } catch (std::exception &e) {
    _logger->error("Failed to create render system: {}", SDL_GetError());
  } catch (...) {
    _logger->error("Failed to create render system: unknown exception");
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
    _logger->error("Failed to create locale: {}", e.what());
  } catch (...) {
    _logger->error("Failed to create locale: unknown exception");
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
  _logger->debug("Application cleaned up");
  SDL_Quit();
}
void Application::onWindowClose() {
  _logger->debug("Window close requested, exiting loop");
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
  _logger->debug("Support languages:");
  for (auto &[key, name] : _locale->getLanguages()) {
    _logger->debug("  key: {}, name: {}", key.c_str(), name.c_str());
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
  _logger->debug("Exiting application");
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
    _logger->error("Could not get base path: {}", SDL_GetError());
    return -1;
  }
  _logger->info("Application running with CWD: {}", _cwd);
  _logger->debug("Application started with options:");
  for (auto &[option, value] : _options) {
    _logger->debug("Option: {} = {}", option, value);
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
    _logger->error("Failed to load asset store: {}assets", _cwd);
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