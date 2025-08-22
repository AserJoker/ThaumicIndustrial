#include "render/Sprite.hpp"
#include "render/Image.hpp"
#include "runtime/Application.hpp"
#include <SDL3/SDL.h>
#include <memory>
void Sprite::setImage(const std::string &name) {
  auto app = Application::getInstance();
  if (_image != name) {
    _image = name;
    SDL_Texture *texture = app->getRenderSystem()->getTexture(name);
    if (!texture) {
      auto asset = app->getAssetManager()->query(name);
      auto image = std::dynamic_pointer_cast<Image>(asset);
      if (image) {
        texture =
            app->getRenderSystem()->createTexture(name, image->getSurface());
      } else {
        _logger->error("Missing texture: {}", name);
      }
    }
    _fragment.setTexture(texture);
    if (texture) {
      _fragment.setRect({0, 0, static_cast<float>(texture->w),
                         static_cast<float>(texture->h)});
      _fragment.setClipRect({0, 0, static_cast<float>(texture->w),
                             static_cast<float>(texture->h)});
    }
  }
}
void Sprite::draw() {
  auto app = Application::getInstance();
  app->getRenderSystem()->draw(&_fragment);
}