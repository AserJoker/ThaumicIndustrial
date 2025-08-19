#include "render/Layer.hpp"
#include <algorithm>

void Layer::add(RenderObject *object) { _renderObjects.push_back(object); }
void Layer::remove(RenderObject *object) { _renderObjects.remove(object); }
bool Layer::has(RenderObject *object) {
  return std::find(_renderObjects.begin(), _renderObjects.end(), object) !=
         _renderObjects.end();
}
void Layer::draw(SDL_Renderer *renderer) const {
  if (!isVisible()) {
    return;
  }
  for (auto &object : _renderObjects) {
    object->draw(renderer);
  }
}