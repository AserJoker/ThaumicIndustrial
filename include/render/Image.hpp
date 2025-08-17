#pragma once
#include "core/Object.hpp"
#include <SDL3/SDL.h>
#include <string>

class Image : public Object {
private:
  SDL_Texture *_texture = nullptr;

public:
  ~Image() override;
  inline SDL_Texture *getTexture() const { return _texture; }
  bool setTexture(const std::string &path);
  bool setTexture(SDL_Texture *texture);
  bool setTexture(uint32_t width, uint32_t height, void *pixels = nullptr,
                  SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA32);
};