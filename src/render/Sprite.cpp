#include "render/Sprite.hpp"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
void Sprite::draw(SDL_Renderer *renderer) const {
  if (!_image || !_image->getTexture()) {
    return;
  }
  if (!isVisible()) {
    return;
  }
  if (!SDL_RenderTextureRotated(renderer, _image->getTexture(), &_clipRect,
                                &_rect, _rotateAngle, &_rotateCenter,
                                _flipMode)) {
    SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed to draw sprite: %s",
                 SDL_GetError());
    return;
  }
}
void Sprite::setImageRect() {
  if (!_image || !_image->getTexture()) {
    return;
  }
  _rect.w = static_cast<float>(_image->getTexture()->w);
  _rect.h = static_cast<float>(_image->getTexture()->h);
}
void Sprite::setImageClipRect() {
  if (!_image || !_image->getTexture()) {
    return;
  }
  _clipRect.w = static_cast<float>(_image->getTexture()->w);
  _clipRect.h = static_cast<float>(_image->getTexture()->h);
}