#pragma once
#include "render/RenderObject.hpp"
#include <list>
class Layer : public RenderObject {
private:
  std::list<RenderObject *> _renderObjects;

public:
  void add(RenderObject *object);
  void remove(RenderObject *object);
  bool has(RenderObject *object);
  inline const std::list<RenderObject *> &getRenderObjects() const {
    return _renderObjects;
  };
  void draw(SDL_Renderer *renderer) const override;
};