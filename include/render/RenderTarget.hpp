#pragma once
#include "core/Object.hpp"
#include "render/Layer.hpp"
#include "runtime/Logger.hpp"
#include <SDL3/SDL_render.h>
#include <memory>
#include <unordered_map>
class RenderTarget : public Object {
private:
  std::unordered_map<size_t, std::unique_ptr<Layer>> _layers;
  Logger *_logger = Logger::getLogger("Render");

public:
  Layer *getLayer(size_t zIndex = 0);
  void draw(SDL_Renderer *renderer, SDL_Texture *target = nullptr);
};