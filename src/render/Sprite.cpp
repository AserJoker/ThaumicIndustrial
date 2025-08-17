#include "render/Sprite.hpp"
#include <SDL3/SDL.h>
#include <memory>
std::shared_ptr<Sprite> Sprite::clone() const {
  auto clonedSprite = std::make_shared<Sprite>();
  clonedSprite->_image = _image;
  clonedSprite->_rect = _rect;
  clonedSprite->_clipRect = _clipRect;
  clonedSprite->_visible = _visible;
  return clonedSprite;
}
Sprite::Sprite(const std::shared_ptr<Image> &image, const SDL_FRect &rect,
               const SDL_FRect &clipRect)
    : _image(image), _rect(rect), _clipRect(clipRect) {
  if (_image && _image->getTexture()) {
    if (clipRect.w == 0 && clipRect.h == 0) {
      _clipRect = {0, 0, static_cast<float>(_image->getTexture()->w),
                   static_cast<float>(_image->getTexture()->h)};
    }
    if (rect.w == 0 && rect.h == 0) {
      _rect = {0, 0, static_cast<float>(_image->getTexture()->w),
               static_cast<float>(_image->getTexture()->h)};
    }
  }
}
void Sprite::setImage(const std::shared_ptr<Image> &image) {
  _image = image;
  if (_image && _image->getTexture()) {
    if (_clipRect.w == 0 && _clipRect.h == 0) {
      _clipRect = {0, 0, static_cast<float>(_image->getTexture()->w),
                   static_cast<float>(_image->getTexture()->h)};
    }
    if (_rect.w == 0 && _rect.h == 0) {
      _rect = {0, 0, static_cast<float>(_image->getTexture()->w),
               static_cast<float>(_image->getTexture()->h)};
    }
  }
}
void Sprite::setRect(const SDL_FRect &rect) {
  _rect = rect;
  if (_rect.w == 0 && _rect.h == 0) {
    _rect = {0, 0, static_cast<float>(_image->getTexture()->w),
             static_cast<float>(_image->getTexture()->h)};
  }
}
void Sprite::setClipRect(const SDL_FRect &clipRect) {
  _clipRect = clipRect;
  if (_clipRect.w == 0 && _clipRect.h == 0) {
    _clipRect = {0, 0, static_cast<float>(_image->getTexture()->w),
                 static_cast<float>(_image->getTexture()->h)};
  }
}
void Sprite::draw(SDL_Renderer *renderer) const {
  if (!_visible || !_image || !_image->getTexture()) {
    return;
  }
  SDL_FRect dstRect = _rect;
  if (_clipRect.w > 0 && _clipRect.h > 0) {
    SDL_RenderTexture(renderer, _image->getTexture(), &_clipRect, &dstRect);
  } else {
    SDL_RenderTexture(renderer, _image->getTexture(), nullptr, &dstRect);
  }
}