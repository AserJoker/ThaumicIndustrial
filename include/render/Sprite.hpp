#pragma once
#include "Fragment.hpp"
#include "core/Object.hpp"
#include "runtime/Logger.hpp"
#include <string>
class Sprite : public Object {
private:
  Fragment _fragment;
  std::string _image;

  Logger *_logger = Logger::getLogger("Render");

public:
  void setImage(const std::string &image);
  const std::string &getImage() const { return _image; }
  void draw();
  inline const SDL_FRect &getRect() const { return _fragment.getRect(); }
  inline void setRect(const SDL_FRect &rect) { _fragment.setRect(rect); }
  inline const SDL_FRect &getClipRect() const {
    return _fragment.getClipRect();
  }
  inline void setClipRect(const SDL_FRect &clipRect) {
    _fragment.setClipRect(clipRect);
  }
  inline const SDL_FPoint &getRotateCenter() const {
    return _fragment.getRotateCenter();
  }
  inline void setRotateCeneter(const SDL_FPoint &center) {
    _fragment.setRotateCeneter(center);
  }
  inline float getRotateAngle() const { return _fragment.getRotateAngle(); }
  inline void setRotateAngle(float angle) { _fragment.setRotateAngle(angle); }
  inline SDL_FlipMode getFlipMode() const { return _fragment.getFlipMode(); }
  inline void setFlipMode(SDL_FlipMode mode) { _fragment.setFlipMode(mode); }
  inline int32_t getZIndex() const { return _fragment.getZIndex(); }
  inline void setZIndex(int32_t zindex) { _fragment.setZIndex(zindex); }
  inline void setPosition(const SDL_FPoint &position) {
    _fragment.setPosition(position);
  }
  inline void setClipPosition(const SDL_FPoint &position) {
    _fragment.setClipPosition(position);
  }
  inline void setRotate(const SDL_FPoint &center, float angle) {
    _fragment.setRotate(center, angle);
  }
};