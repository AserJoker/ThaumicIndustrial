#pragma once
#include "core/Object.hpp"
#include <SDL3/SDL.h>
class RenderObject : public Object {
private:
  bool _visible = true;

public:
  inline void setVisible(bool visible) { _visible = visible; }
  inline bool isVisible() const { return _visible; }
  virtual void draw(SDL_Renderer *renderer) const = 0;
};