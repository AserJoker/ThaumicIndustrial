#pragma once
#include "core/Object.hpp"
#include <SDL3/SDL.h>
class RenderObject : public Object {
public:
  virtual void draw(SDL_Renderer *renderer) const = 0;
};