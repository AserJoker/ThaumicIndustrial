#include "runtime/Application.hpp"
#include "core/ScopeGuard.hpp"
#include "render/RenderSystem.hpp"
#include "runtime/AssetManager.hpp"
#include "runtime/ImageLoader.hpp"
#include "runtime/JsonLoader.hpp"
#include "runtime/LocaleManager.hpp"
#include "runtime/Logger.hpp"
#include "runtime/SaveManager.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
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
    _assetManager.reset(new AssetManager());
    _assetManager->registerLoader("png", imgLoader);
    _assetManager->registerLoader("bmp", imgLoader);
    _assetManager->registerLoader("jpeg", imgLoader);
    _assetManager->registerLoader("jpg", imgLoader);
    auto jsonLoader = std::make_shared<JsonLoader>();
    _assetManager->registerLoader("json", jsonLoader);
    if (!_assetManager->initStore(_cwd + "assets")) {
      _logger->error("Failed to load asset store: {}assets", _cwd);
    }
    return true;
  } catch (std::exception &e) {
    _logger->error("Failed to create asset manager: {}", SDL_GetError());
  } catch (...) {
    _logger->error("Failed to create asset manager: unknown exception");
  }
  return false;
}
bool Application::initConfigManager() {
  try {
    _configManager.reset(new ConfigManager());
    return true;
  } catch (std::exception &e) {
    _logger->error("Failed to create config manager: {}", SDL_GetError());
  } catch (...) {
    _logger->error("Failed to create config manager: unknown exception");
  }
  return false;
}

bool Application::initSaveManager() {
  try {
    _saveManager.reset(new SaveManager());
    return true;
  } catch (std::exception &e) {
    _logger->error("Failed to create save manager: {}", SDL_GetError());
  } catch (...) {
    _logger->error("Failed to create save manager: unknown exception");
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
    _renderSystem.reset(new RenderSystem(renderer));
    SDL_Texture *texture =
        _renderSystem->createTexture("system.texture.missing", 2, 2);
    if (!texture) {
      return false;
    }
    uint32_t data[] = {0xff000000, 0xffffffff, 0xffffffff, 0xff000000};
    SDL_UpdateTexture(texture, nullptr, data, 2 * sizeof(uint32_t));
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    return true;
  } catch (std::exception &e) {
    _logger->error("Failed to create render system: {}", SDL_GetError());
  } catch (...) {
    _logger->error("Failed to create render system: unknown exception");
  }
  return false;
}
bool Application::initLocaleManager() {
  try {
    _localeManager.reset(new LocaleManager());
    _localeManager->resolveManifest("thaumicindustrial.locale.manifest");
    _localeManager->setDefaultLang("en_US");
    _localeManager->setLang("en_US");
    return true;
  } catch (std::exception &e) {
    _logger->error("Failed to create locale: {}", e.what());
  } catch (...) {
    _logger->error("Failed to create locale: unknown exception");
  }
  return false;
}
bool Application::initModManager() {
  try {
    _modManager.reset(new ModManager());
    _modManager->loadAllMod();
    return true;
  } catch (std::exception &e) {
    _logger->error("Failed to create mod manager: {}", e.what());
  } catch (...) {
    _logger->error("Failed to create mod manager: unknown exception");
  }
  return false;
}

void Application::cleanup() {
  if (_renderSystem) {
    _renderSystem.reset(nullptr);
  }
  if (_window) {
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }
  TTF_Quit();
  _logger->debug("Application cleaned up");
  SDL_Quit();
}
void Application::onWindowClose(const SDL_WindowEvent &event) {
  _logger->debug("Window close requested, exiting loop");
  _running = false;
}
void Application::onWindowResize(const SDL_WindowEvent &event) {}
void Application::onWindowFocusGained(const SDL_WindowEvent &event) {}
void Application::onWindowFocusLost(const SDL_WindowEvent &event) {}
void Application::onMouseButtonDown(const SDL_MouseButtonEvent &event) {}
void Application::onMouseButtonUp(const SDL_MouseButtonEvent &event) {}
void Application::onKeyDown(const SDL_KeyboardEvent &event) {}
void Application::onKeyUp(const SDL_KeyboardEvent &event) {}

bool Application::processEvent() {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
      onWindowClose(event.window);
      break;
    case SDL_EVENT_WINDOW_RESIZED:
      onWindowResize(event.window);
      break;
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
      onWindowFocusGained(event.window);
      break;
    case SDL_EVENT_WINDOW_FOCUS_LOST:
      onWindowFocusLost(event.window);
      break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      onMouseButtonDown(event.button);
      break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
      onMouseButtonUp(event.button);
      break;
    case SDL_EVENT_KEY_DOWN:
      onKeyDown(event.key);
      break;
    case SDL_EVENT_KEY_UP:
      onKeyUp(event.key);
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
  for (auto &[code, locales] : _localeManager->getLanguages()) {
    _logger->debug("  code: {}", code.c_str());
    for (auto &locale : locales) {
      _logger->debug("    =============================================\n");
      _logger->debug("    name: {}\n", locale.name.c_str());
      _logger->debug("    description: {}\n", locale.description.c_str());
      _logger->debug("    version: {}\n", locale.version.c_str());
      _logger->debug("    author: {}\n", locale.author.c_str());
      _logger->debug("    license: {}\n", locale.license.c_str());
      _logger->debug("    supported: {}\n", locale.supported.c_str());
      _logger->debug("    asset: {}\n", locale.asset.c_str());
      _logger->debug("    mod: {}\n", locale.mod.c_str());
    }
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
  if (!initConfigManager()) {
    return -1;
  }
  if (!initSaveManager()) {
    return -1;
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
  if (!initLocaleManager()) {
    return -1;
  }
  if (!initModManager()) {
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