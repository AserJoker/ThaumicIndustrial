#include "runtime/ImageLoader.hpp"
#include "core/Object.hpp"
#include "runtime/Application.hpp"
#include <SDL3_image/SDL_image.h>
#include <memory>
std::shared_ptr<Object> ImageLoader::load(const std::string &path) {
  SDL_Surface *surface = IMG_Load(path.c_str());
  if (!surface) {
    return nullptr;
  }
  auto app = Application::getInstance();
  auto img = app->getRenderSystem()->createImage(surface);
  SDL_DestroySurface(surface);
  return std::shared_ptr<Object>(img);
}