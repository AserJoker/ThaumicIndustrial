#pragma once
#include "SDL3/SDL_video.h"
#include "core/Object.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <unordered_map>

class Application : public Object {
private:
  static std::unique_ptr<Application> _instance;

private:
  std::unordered_map<std::string, std::string> _options;
  SDL_Window *_window = nullptr;
  SDL_Renderer *_renderer = nullptr;
  bool _running = true;
  std::string _cwd;

private:
  void resolveOptions(int argc, char **argv);
  void initLog();
  bool createWindowAndRenderer();
  void cleanup();
  bool processEvent();
  void onUpdate();

public:
  int run(int argc, char **argv);
  const std::string &getOption(const std::string &key,
                               const std::string &def = "") const;
  ~Application() override;
  void exit();
  inline SDL_Window *getWindow() const { return _window; }
  inline SDL_Renderer *getRenderer() const { return _renderer; }
  const std::string &getCWD() const { return _cwd; }

public:
  static Application &getInstance() {
    if (!_instance) {
      _instance = std::make_unique<Application>();
    }
    return *_instance;
  }
};