#include "render/TileMap.hpp"
#include <SDL3/SDL_render.h>
void TileMap::draw(SDL_Renderer *renderer) const {
  if (!_getRect || !_getClipRect || !_image) {
    return;
  }
  for (uint32_t idx = 0; idx < _data.size(); idx++) {
    auto &tile = _data[idx];
    if (tile) {
      auto rect = _getRect(_image, idx, tile);
      auto clipRect = _getClipRect(_image, idx, tile);
      SDL_RenderTexture(renderer, _image->getTexture(), &clipRect, &rect);
    }
  }
}