#include "render/RenderSystem.hpp"
std::unique_ptr<RendererSystem> RendererSystem::_instance = nullptr;
Layer *RendererSystem::getLayer(int32_t zIndex) {
  auto it = _layers.find(zIndex);
  if (it != _layers.end()) {
    return it->second.get();
  }
  auto layer = std::make_unique<Layer>();
  layer->setZIndex(zIndex);
  Layer *layerPtr = layer.get();
  _layers[zIndex] = std::move(layer);
  SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Created new layer with zIndex: %d",
               zIndex);
  return layerPtr;
}
bool RendererSystem::removeLayer(int32_t zIndex) {
  auto it = _layers.find(zIndex);
  if (it != _layers.end()) {
    _layers.erase(it);
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Removed layer with zIndex: %d",
                 zIndex);
    return true;
  }
  return false;
}
bool RendererSystem::hasLayer(int32_t zIndex) const {
  return _layers.find(zIndex) != _layers.end();
}
void RendererSystem::clearLayers() {
  _layers.clear();
  SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Cleared all layers");
}
void RendererSystem::draw(SDL_Renderer *renderer) const {
  if (!renderer) {
    return;
  }
  for (const auto &[_, layer] : _layers) {
    layer->draw(renderer);
  }
}