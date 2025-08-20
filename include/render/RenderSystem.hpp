#pragma once
#include "Image.hpp"
#include "core/Object.hpp"
#include "render/RenderTarget.hpp"
#include <SDL3/SDL.h>
#include <memory>
class RenderSystem : public Object {
private:
  SDL_Renderer *_renderer = {};
  std::unique_ptr<RenderTarget> _renderTarget;

public:
  RenderSystem(SDL_Renderer *renderer);
  ~RenderSystem() override;

  void present();

  inline RenderTarget *getRenderTarget() const { return _renderTarget.get(); }

  std::shared_ptr<Image>
  createImage(size_t width, size_t height,
              SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA32,
              SDL_TextureAccess access = SDL_TEXTUREACCESS_STATIC);

  std::shared_ptr<Image> createImage(SDL_Surface *surface);
};