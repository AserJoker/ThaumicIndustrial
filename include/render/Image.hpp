#pragma once
#include "core/Object.hpp"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
class Image : public Object {
private:
  SDL_Texture *_texture = {};

public:
  Image(SDL_Texture *texture);
  ~Image() override;
  inline SDL_Texture *getTexture() const { return _texture; }
  void setTexture(SDL_Texture *texture);
};