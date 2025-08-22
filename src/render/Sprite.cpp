#include "render/Sprite.hpp"
#include "runtime/Application.hpp"
#include <SDL3/SDL.h>
void Sprite::setImage(const std::string &name) {
  auto app = Application::getInstance();
  if (_fragment.getTexture() != name) {
    _fragment.setTexture(name);
  }
}
void Sprite::draw(RenderSystem *renderSystem) {
  renderSystem->draw(&_fragment);
}