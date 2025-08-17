#pragma once
#include "core/Object.hpp"
#include "render/Layer.hpp"
#include <cstdint>
#include <memory>
#include <unordered_map>
class RendererSystem : public Object {
private:
  static std::unique_ptr<RendererSystem> _instance;

public:
  static RendererSystem &getInstance() {
    if (!_instance) {
      _instance = std::make_unique<RendererSystem>();
    }
    return *_instance;
  }

private:
  std::unordered_map<int32_t, std::unique_ptr<Layer>> _layers;

public:
  Layer *getLayer(int32_t zIndex = 0);
  bool removeLayer(int32_t zIndex);
  bool hasLayer(int32_t zIndex) const;
  void clearLayers();
  void draw(SDL_Renderer *renderer) const;
  inline const std::unordered_map<int32_t, std::unique_ptr<Layer>> &
  getLayers() const {
    return _layers;
  }
};