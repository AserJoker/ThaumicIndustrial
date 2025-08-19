#include "render/Image.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
Image::Image(SDL_Texture *texture) : _texture(texture) {}

Image::~Image() {
  if (_texture) {
    SDL_DestroyTexture(_texture);
    _texture = nullptr;
  }
}

void Image::setTexture(SDL_Texture *texture) {
  if (_texture) {
    SDL_DestroyTexture(_texture);
    _texture = nullptr;
  }
  _texture = texture;
}