#pragma once
#include "render/Image.hpp"
#include "render/RenderObject.hpp"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>
class TileMap : public RenderObject {
public:
  using TileRectGenerator = std::function<SDL_FRect(
      const std::shared_ptr<Image> &image, uint32_t idx, uint32_t data)>;

private:
  std::shared_ptr<Image> _image;
  TileRectGenerator _getRect;
  TileRectGenerator _getClipRect;
  std::vector<size_t> _data;

public:
  inline void setImage(const std::shared_ptr<Image> &image) { _image = image; };
  inline const std::shared_ptr<Image> &getImage() const { return _image; }
  inline void setGetRectFunc(const TileRectGenerator &getRect) {
    _getRect = getRect;
  }
  inline void setGetClipRectFunc(const TileRectGenerator &getClipRect) {
    _getClipRect = getClipRect;
  }
  inline void setData(const std::vector<size_t> &data) { _data = data; }
  inline std::vector<size_t> &getData() { return _data; }
  inline const std::vector<size_t> &getData() const { return _data; }
  inline void set(uint32_t idx, uint32_t data) {
    if (idx >= _data.size()) {
      _data.resize(idx + 1, 0);
    }
    _data[idx] = data;
  };
  void draw(SDL_Renderer *renderer) const override;
};