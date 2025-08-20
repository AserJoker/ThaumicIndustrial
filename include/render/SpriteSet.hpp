#pragma once
#include "RenderTarget.hpp"
#include "render/RenderObject.hpp"
#include "render/Sprite.hpp"
#include <SDL3/SDL_render.h>
#include <memory>
class SpriteSet : public RenderObject {
private:
  std::unique_ptr<RenderTarget> _renderTarget =
      std::make_unique<RenderTarget>();
  std::unique_ptr<Sprite> _sprite = std::make_unique<Sprite>();

public:
  inline Sprite *getSprite() const { return _sprite.get(); }
  inline RenderTarget *getRenderTarget() const { return _renderTarget.get(); }
  void draw(SDL_Renderer *renderer) const override;
};