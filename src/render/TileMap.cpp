#include "render/TileMap.hpp"
void TileMap::draw(RenderSystem *renderSystem) {
  if (_dirty) {
    _fragements.clear();
    auto texture = renderSystem->getTexture(_texture);
    if (!texture) {
      texture = renderSystem->getTexture("system.texture.missing");
    }
    auto [width, height] = _tileSize;
    auto tileWidth = texture->w / width;
    auto tileHeight = texture->h / height;
    if (tileWidth == 0) {
      tileWidth = 1;
      width = texture->w;
    }
    if (tileHeight == 0) {
      tileHeight = 1;
      height = texture->h;
    }
    for (uint32_t y = 0; y < _size.second; ++y) {
      for (uint32_t x = 0; x < _size.first; ++x) {
        auto tile = getTile(x, y);
        if (tile == 0) {
          continue;
        }
        auto tileX = (tile - 1) % tileWidth;
        auto tileY = (tile - 1) / tileWidth;
        auto idx = y * _size.first + x;
        auto &fragment = _fragements[idx];
        fragment.setTexture(_texture);
        fragment.setRect({
            static_cast<float>(x * _tileSize.first),
            static_cast<float>(y * _tileSize.second),
            static_cast<float>(_tileSize.first),
            static_cast<float>(_tileSize.second),
        });
        fragment.setClipRect({
            static_cast<float>(tileX * width),
            static_cast<float>(tileY * height),
            static_cast<float>(width),
            static_cast<float>(height),
        });
      }
    }
    _dirty = false;
  }
  for (auto &[_, fragment] : _fragements) {
    renderSystem->draw(&fragment);
  }
}