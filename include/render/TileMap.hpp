#pragma once
#include "core/Object.hpp"
#include "render/Fragment.hpp"
#include "render/RenderSystem.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
class TileMap : public Object {
private:
  std::pair<uint32_t, uint32_t> _size;
  std::pair<uint32_t, uint32_t> _tileSize;
  std::vector<uint32_t> _tiles;
  std::unordered_map<size_t, Fragment> _fragements;
  std::string _texture = "system.texture.missing";
  bool _dirty = false;

public:
  inline const std::pair<uint32_t, uint32_t> &getSize() const { return _size; }
  inline void setSize(const std::pair<uint32_t, uint32_t> &size) {
    _size = size;
    _tiles.resize(_size.first * _size.second, 0);
    _fragements.clear();
    _dirty = true;
  }
  inline const std::pair<uint32_t, uint32_t> &getTileSize() const {
    return _tileSize;
  }
  inline void setTileSize(const std::pair<uint32_t, uint32_t> &tileSize) {
    _tileSize = tileSize;
    _dirty = true;
  }
  inline const std::string &getTexture() const { return _texture; }
  inline void setTexture(const std::string &texture) {
    _texture = texture;
    _dirty = true;
  }
  inline uint32_t getTile(uint32_t x, uint32_t y) const {
    if (x >= _size.first || y >= _size.second) {
      return 0;
    }
    return _tiles[y * _size.first + x];
  }
  inline void setTile(uint32_t x, uint32_t y, uint32_t tile) {
    if (x >= _size.first || y >= _size.second) {
      return;
    }
    auto idx = y * _size.first + x;
    _tiles[idx] = tile;
    _dirty = true;
  }
  void draw(RenderSystem *renderSystem);
};