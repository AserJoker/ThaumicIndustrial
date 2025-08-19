#include "render/RenderSystem.hpp"
#include "render/Image.hpp"
#include "render/RenderTarget.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <cstddef>
#include <memory>
RenderSystem::RenderSystem(SDL_Renderer *renderer) : _renderer(renderer) {
  SDL_SetRenderDrawColorFloat(_renderer, 0.2, 0.3, 0.3, 1.0);
  _renderTarget = std::make_unique<RenderTarget>();
}

void RenderSystem::present() {
  if (!_renderer) {
    return;
  }
  SDL_RenderClear(_renderer);
  _renderTarget->draw(_renderer);
  SDL_RenderPresent(_renderer);
}
Image *RenderSystem::createImage(size_t width, size_t height,
                                 SDL_PixelFormat format,
                                 SDL_TextureAccess access) {
  SDL_Texture *texture =
      SDL_CreateTexture(_renderer, format, access, width, height);
  if (!texture) {
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed to create texture: %s",
                 SDL_GetError());
    return nullptr;
  }
  return new Image(texture);
}

Image *RenderSystem::createImage(SDL_Surface *surface) {
  SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
  if (!texture) {
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed to create texture: %s",
                 SDL_GetError());
    return nullptr;
  }
  return new Image(texture);
}