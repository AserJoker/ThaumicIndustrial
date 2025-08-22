#include "render/RenderSystem.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
RenderSystem::RenderSystem(SDL_Renderer *renderer) : _renderer(renderer) {
  SDL_SetRenderDrawColorFloat(_renderer, 0.2, 0.3, 0.3, 1.0);
}
RenderSystem::~RenderSystem() {
  if (_renderer) {
    for (auto &[_, texture] : _textures) {
      SDL_DestroyTexture(texture);
    }
    _textures.clear();
    SDL_DestroyRenderer(_renderer);
    _renderer = nullptr;
  }
}
void RenderSystem::draw(Fragment *fragment) {
  _fragements[fragment->getZIndex()].push_back(fragment);
}

void RenderSystem::present() {
  if (!_renderer) {
    return;
  }
  SDL_RenderClear(_renderer);
  for (auto &[_, fragments] : _fragements) {
    for (auto &fragment : fragments) {
      SDL_Texture *texture = fragment->getTexture();
      if (!texture) {
        texture = _textures.at("system.texture.missing");
      }
      SDL_RenderTextureRotated(
          _renderer, fragment->getTexture(), &fragment->getClipRect(),
          &fragment->getRect(), fragment->getRotateAngle(),
          &fragment->getRotateCenter(), fragment->getFlipMode());
    }
    fragments.clear();
  }
  SDL_RenderPresent(_renderer);
}
SDL_Texture *RenderSystem::createTexture(const std::string &name,
                                         SDL_Surface *surface) {
  removeTexture(name);
  SDL_Texture *tex = SDL_CreateTextureFromSurface(_renderer, surface);
  if (!tex) {
    _logger->error("Failed to create texture '{}': {}", name, SDL_GetError());
    return nullptr;
  }
  _textures[name] = tex;
  return tex;
}
SDL_Texture *RenderSystem::createTexture(const std::string &name, uint32_t w,
                                         uint32_t h, SDL_PixelFormat format,
                                         SDL_TextureAccess access) {
  removeTexture(name);
  SDL_Texture *tex = SDL_CreateTexture(_renderer, format, access, w, h);
  if (!tex) {
    _logger->error("Failed to create texture '{}': {}", name, SDL_GetError());
    return nullptr;
  }
  _textures[name] = tex;
  return tex;
}

SDL_Texture *RenderSystem::getTexture(const std::string &name) {
  if (_textures.contains(name)) {
    return _textures.at(name);
  }
  return nullptr;
}
void RenderSystem::removeTexture(const std::string &name) {
  if (_textures.contains(name)) {
    SDL_DestroyTexture(_textures.at(name));
    _textures.erase(name);
  }
}