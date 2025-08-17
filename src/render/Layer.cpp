#include "render/Layer.hpp"
#include <SDL3/SDL_log.h>
#include <algorithm>
bool Layer::addRenderObject(const std::shared_ptr<RenderObject> &RenderObject) {
  if (!RenderObject) {
    return false;
  }
  _renderObjects.push_back(RenderObject);
  SDL_LogDebug(SDL_LOG_CATEGORY_RENDER,
               "Added render object to layer with zIndex: %d", _zIndex);
  return true;
}
bool Layer::removeRenderObject(
    const std::shared_ptr<RenderObject> &RenderObject) {
  if (!RenderObject) {
    return false;
  }
  auto it =
      std::find(_renderObjects.begin(), _renderObjects.end(), RenderObject);
  if (it != _renderObjects.end()) {
    _renderObjects.erase(it);
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER,
                 "Removed render object from layer with zIndex: %d", _zIndex);
    return true;
  }
  return false;
}
bool Layer::hasRenderObject(
    const std::shared_ptr<RenderObject> &RenderObject) const {
  if (!RenderObject) {
    return false;
  }
  return std::find(_renderObjects.begin(), _renderObjects.end(),
                   RenderObject) != _renderObjects.end();
}
void Layer::clearRenderObjects() {
  _renderObjects.clear();
  SDL_LogDebug(SDL_LOG_CATEGORY_RENDER,
               "Cleared all render object from layer with zIndex: %d", _zIndex);
}
void Layer::draw(SDL_Renderer *renderer) const {
  if (!renderer) {
    return;
  }
  for (const auto &RenderObject : _renderObjects) {
    RenderObject->draw(renderer);
  }
}