#pragma once
#include "AssetManager.hpp"
#include "Locale.hpp"
#include "core/Object.hpp"
#include "render/RenderSystem.hpp"
#include "runtime/Logger.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <unordered_map>

class Application : public Object {
public:
  static inline const std::string APP_NAME = "thaumicindustrial";

private:
  static std::unique_ptr<Application> _instance;

private:
  std::unordered_map<std::string, std::string> _options;
  std::unordered_map<int, std::string> _logCategoryNames;
  SDL_Window *_window = nullptr;
  AssetManager *_assetManager = nullptr;
  RenderSystem *_renderSystem = nullptr;
  Locale *_locale = nullptr;
  bool _running = true;
  std::string _cwd;
  Logger *_logger = Logger::getLogger("Application");

private:
  void resolveOptions(int argc, char **argv);
  void initLog();
  bool createWindow();
  bool initAssetManager();
  bool initRenderSystem();
  bool initLocale();
  void cleanup();
  bool processEvent();

private:
  void onPreInitialize();
  void onInitialize();
  void onPostInitialize();
  void onUpdate();
  void onWindowClose();
  void onUninitialize();

public:
  int run(int argc, char **argv);
  const std::string &getOption(const std::string &key,
                               const std::string &def = "") const;
  ~Application() override;
  void exit();
  inline SDL_Window *getWindow() const { return _window; }
  const std::string &getCWD() const { return _cwd; }
  inline RenderSystem *getRenderSystem() const { return _renderSystem; }
  inline AssetManager *getAssetManager() const { return _assetManager; }
  inline Locale *getLocale() const { return _locale; }

public:
  static Application *getInstance() {
    if (!_instance) {
      _instance = std::make_unique<Application>();
    }
    return _instance.get();
  }
};