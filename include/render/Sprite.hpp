#pragma once
#include "Image.hpp"
#include "RenderObject.hpp"
#include <SDL3/SDL.h>
#include <memory>

class Sprite : public RenderObject {
private:
  std::shared_ptr<Image> _image = nullptr;
  SDL_FRect _rect = {0, 0, 0, 0};
  SDL_FRect _clipRect = {0, 0, 0, 0};
  SDL_FlipMode _flipMode = SDL_FLIP_NONE;
  SDL_FPoint _rotateCenter = {0, 0};
  float _rotateAngle = .0f;

public:
  inline std::shared_ptr<Image> getImage() const { return _image; }
  inline const SDL_FRect &getRect() const { return _rect; }
  inline const SDL_FRect &getClipRect() const { return _clipRect; }
  inline SDL_FlipMode getFlipMode() const { return _flipMode; }
  inline const SDL_FPoint &getRotateCenter() const { return _rotateCenter; };
  inline float getRotateAngle() const { return _rotateAngle; }

  inline void setImage(const std::shared_ptr<Image> &image) {
    _image = image;
    setImageRect();
    setImageClipRect();
  }
  inline void setRect(const SDL_FRect &rect) { _rect = rect; }
  inline void setClipRect(const SDL_FRect &rect) { _clipRect = rect; }
  inline void setFlipMode(SDL_FlipMode flipMode) { _flipMode = flipMode; }
  inline void setRotateCenter(const SDL_FPoint &center) {
    _rotateCenter = center;
  }
  inline void setRotateAngle(float angle) { _rotateAngle = angle; }
  inline void setRotate(const SDL_FPoint &center, float angle) {
    _rotateCenter = center;
    _rotateAngle = angle;
  }
  inline void setPosition(const SDL_FPoint &position) {
    _rect.x = position.x;
    _rect.y = position.y;
  }
  inline void setClipPosition(const SDL_FPoint &position) {
    _clipRect.x = position.x;
    _clipRect.y = position.y;
  }
  void draw(SDL_Renderer *renderer) const override;
  void setImageRect();
  void setImageClipRect();
};