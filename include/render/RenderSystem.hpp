#pragma once
#include "Fragment.hpp"
#include "core/Object.hpp"
#include "runtime/Logger.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
class RenderSystem : public Object {
private:
  Logger *_logger = Logger::getLogger("Render");

  SDL_Renderer *_renderer = {};
  std::unordered_map<int32_t, std::vector<Fragment *>> _fragements;
  std::unordered_map<std::string, SDL_Texture *> _textures;

public:
  RenderSystem(SDL_Renderer *renderer);
  ~RenderSystem() override;
  void draw(Fragment *fragment);
  void present();
  SDL_Texture *createTexture(const std::string &name, SDL_Surface *surface);
  SDL_Texture *
  createTexture(const std::string &name, uint32_t w, uint32_t h,
                SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA32,
                SDL_TextureAccess access = SDL_TEXTUREACCESS_STATIC);
  SDL_Texture *getTexture(const std::string &name);
  void removeTexture(const std::string &name);
};