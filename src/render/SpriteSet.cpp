#include "render/SpriteSet.hpp"
#include <SDL3/SDL_render.h>
void SpriteSet::draw(SDL_Renderer *renderer) const {
  if (!_sprite->getImage() || !isVisible()) {
    return;
  }
  _renderTarget->draw(renderer, _sprite->getImage()->getTexture());
  _sprite->draw(renderer);
}