#include "render/Image.hpp"
#include "runtime/Application.hpp"
#include <SDL3_image/SDL_image.h>
Image::~Image() {
  if (_texture) {
    SDL_DestroyTexture(_texture);
    _texture = nullptr;
  }
}
bool Image::setTexture(const std::string &path) {
  if (_texture) {
    SDL_DestroyTexture(_texture);
  }
  auto renderer = Application::getInstance().getRenderer();
  if (!renderer) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer is not initialized");
    return false;
  }
  auto surface = IMG_Load(path.c_str());
  if (!surface) {
    return false;
  }
  _texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);
  if (!_texture) {
    return false;
  }
  return true;
}
bool Image::setTexture(SDL_Texture *texture) {
  if (_texture) {
    SDL_DestroyTexture(_texture);
  }
  _texture = texture;
  return true;
}
bool Image::setTexture(uint32_t width, uint32_t height, void *pixels,
                       SDL_PixelFormat format) {
  if (_texture) {
    SDL_DestroyTexture(_texture);
  }
  auto renderer = Application::getInstance().getRenderer();
  if (!renderer) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer is not initialized");
    return false;
  }
  _texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STATIC,
                               width, height);
  if (!_texture) {
    return false;
  }
  if (pixels) {
    SDL_UpdateTexture(_texture, nullptr, pixels, width * sizeof(uint32_t));
  }
  return true;
}