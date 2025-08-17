#pragma once
#include "RenderObject.hpp"
#include "render/Image.hpp"
#include <memory>
class Sprite : public RenderObject {
private:
  std::shared_ptr<Image> _image;
  SDL_FRect _rect = {0, 0, 0, 0};
  SDL_FRect _clipRect = {0, 0, 0, 0};
  bool _visible = true;

public:
  Sprite() = default;
  Sprite(const std::shared_ptr<Image> &image,
         const SDL_FRect &rect = {0, 0, 0, 0},
         const SDL_FRect &clipRect = {0, 0, 0, 0});
  std::shared_ptr<Sprite> clone() const;
  void setImage(const std::shared_ptr<Image> &image);
  inline const std::shared_ptr<Image> &getImage() const { return _image; }
  void setRect(const SDL_FRect &rect = {});
  inline const SDL_FRect &getRect() const { return _rect; }
  void setClipRect(const SDL_FRect &clipRect = {});
  inline const SDL_FRect &getClipRect() const { return _clipRect; }
  void setVisible(bool visible);
  inline bool isVisible() const { return _visible; }
  void draw(SDL_Renderer *renderer) const override;
};