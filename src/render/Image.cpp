#include "render/Image.hpp"
#include <SDL3/SDL.h>
Image::Image(SDL_Surface *surface) : _surface(surface) {}

Image::Image(uint32_t w, uint32_t h, SDL_PixelFormat format, void *data) {
  _surface = SDL_CreateSurface(w, h, format);
  memcpy(_surface->pixels, data, _surface->pitch * h);
}

Image::~Image() {
  if (_surface) {
    SDL_DestroySurface(_surface);
    _surface = nullptr;
  }
}

void Image::setSurface(SDL_Surface *surface) {
  if (_surface) {
    SDL_DestroySurface(_surface);
  }
  _surface = surface;
}