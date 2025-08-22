#pragma once
#include "AssetManager.hpp"
#include "ConfigManager.hpp"
#include "Locale.hpp"
#include "SaveManager.hpp"
#include "core/Object.hpp"
#include "render/RenderSystem.hpp"
#include "runtime/Logger.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <memory>
#include <unordered_map>

class Application : public Object {
public:
  static inline const std::string APP_NAME = "thaumicindustrial";

private:
  static std::unique_ptr<Application> _instance;

private:
  Logger *_logger = Logger::getLogger("Application");

  std::string _cwd;
  std::unordered_map<std::string, std::string> _options;

  bool _running = true;
  SDL_Window *_window = nullptr;

  std::unique_ptr<RenderSystem> _renderSystem;
  std::unique_ptr<Locale> _locale;
  std::unique_ptr<AssetManager> _assetManager;
  std::unique_ptr<ConfigManager> _configManager;
  std::unique_ptr<SaveManager> _saveManager;

private:
  void resolveOptions(int argc, char **argv);
  void initLog();
  bool createWindow();
  bool initAssetManager();
  bool initRenderSystem();
  bool initConfigManager();
  bool initSaveManager();
  bool initLocale();
  void cleanup();
  bool processEvent();

private:
  void onPreInitialize();
  void onInitialize();
  void onPostInitialize();
  void onUpdate();
  void onUninitialize();
  void onWindowClose(const SDL_WindowEvent &event);
  void onWindowResize(const SDL_WindowEvent &event);
  void onWindowFocusGained(const SDL_WindowEvent &event);
  void onWindowFocusLost(const SDL_WindowEvent &event);
  void onMouseButtonDown(const SDL_MouseButtonEvent &event);
  void onMouseButtonUp(const SDL_MouseButtonEvent &event);
  void onKeyDown(const SDL_KeyboardEvent &event);
  void onKeyUp(const SDL_KeyboardEvent &event);

public:
  int run(int argc, char **argv);
  const std::string &getOption(const std::string &key,
                               const std::string &def = "") const;
  ~Application() override;
  void exit();
  inline SDL_Window *getWindow() const { return _window; }
  const std::string &getCWD() const { return _cwd; }
  inline RenderSystem *getRenderSystem() const { return _renderSystem.get(); }
  inline AssetManager *getAssetManager() const { return _assetManager.get(); }
  inline ConfigManager *getConfigManager() const {
    return _configManager.get();
  }
  inline SaveManager *getSaveManager() const { return _saveManager.get(); }
  inline Locale *getLocale() const { return _locale.get(); }

public:
  static Application *getInstance() {
    if (!_instance) {
      _instance = std::make_unique<Application>();
    }
    return _instance.get();
  }
};