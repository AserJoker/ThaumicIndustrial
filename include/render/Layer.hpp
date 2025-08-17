#pragma once
#include "RenderObject.hpp"
#include "core/Object.hpp"
#include <memory>
#include <vector>

class Layer : public Object {
private:
  std::vector<std::shared_ptr<RenderObject>> _renderObjects;
  int32_t _zIndex = 0;

public:
  Layer() = default;
  inline int32_t getZIndex() const { return _zIndex; }
  inline void setZIndex(int32_t zIndex) { _zIndex = zIndex; }
  inline const std::vector<std::shared_ptr<RenderObject>> &
  getRenderObjects() const {
    return _renderObjects;
  }
  bool addRenderObject(const std::shared_ptr<RenderObject> &RenderObject);
  bool removeRenderObject(const std::shared_ptr<RenderObject> &RenderObject);
  bool hasRenderObject(const std::shared_ptr<RenderObject> &RenderObject) const;
  void clearRenderObjects();
  void draw(SDL_Renderer *renderer) const;
};