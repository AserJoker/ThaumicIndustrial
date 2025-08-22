#include "runtime/ImageLoader.hpp"
#include "core/Object.hpp"
#include "render/Image.hpp"
#include <SDL3_image/SDL_image.h>
#include <memory>
std::shared_ptr<Object> ImageLoader::load(const std::string &path) {
  SDL_Surface *surface = IMG_Load(path.c_str());
  if (!surface) {
    return nullptr;
  }
  return std::make_shared<Image>(surface);
}