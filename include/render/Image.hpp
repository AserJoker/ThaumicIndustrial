#pragma once
#include "core/Object.hpp"
#include <SDL3/SDL.h>
class Image : public Object {
private:
  SDL_Surface *_surface = {};

public:
  Image(SDL_Surface *texture = nullptr);
  Image(uint32_t w, uint32_t h, SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA32,
        void *data = nullptr);
  ~Image() override;
  inline SDL_Surface *getSurface() const { return _surface; }
  void setSurface(SDL_Surface *surface);
};