#pragma once
#include "core/Object.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
class Fragment : public Object {
private:
  SDL_FRect _rect = {};
  SDL_FRect _clipRect = {};
  SDL_FPoint _center = {};
  float _angle = 0.f;
  SDL_FlipMode _mode = SDL_FLIP_NONE;
  SDL_Texture *_texture = nullptr;
  int32_t _zIndex = 0;

public:
  inline const SDL_FRect &getRect() const { return _rect; }
  inline void setRect(const SDL_FRect &rect) { _rect = rect; }
  inline const SDL_FRect &getClipRect() const { return _clipRect; }
  inline void setClipRect(const SDL_FRect &clipRect) { _clipRect = clipRect; }
  inline const SDL_FPoint &getRotateCenter() const { return _center; }
  inline void setRotateCeneter(const SDL_FPoint &center) { _center = center; }
  inline float getRotateAngle() const { return _angle; }
  inline void setRotateAngle(float angle) { _angle = angle; }
  inline SDL_FlipMode getFlipMode() const { return _mode; }
  inline void setFlipMode(SDL_FlipMode mode) { _mode = mode; }
  inline int32_t getZIndex() const { return _zIndex; }
  inline void setZIndex(int32_t zindex) { _zIndex = zindex; }
  inline SDL_Texture *getTexture() const { return _texture; }
  inline void setTexture(SDL_Texture *texture) { _texture = texture; }
  inline void setPosition(const SDL_FPoint &position) {
    _rect.x = position.x;
    _rect.y = position.y;
  }
  inline void setClipPosition(const SDL_FPoint &position) {
    _clipRect.x = position.x;
    _clipRect.y = position.y;
  }
  inline void setRotate(const SDL_FPoint &center, float angle) {
    _center = center;
    _angle = angle;
  }
};