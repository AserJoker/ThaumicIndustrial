#include "render/RenderTarget.hpp"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

Layer *RenderTarget::getLayer(size_t zIndex) {
  auto &layer = _layers[zIndex];
  if (!layer) {
    layer.reset(new Layer());
  }
  return layer.get();
}
void RenderTarget::draw(SDL_Renderer *renderer, SDL_Texture *texture) {
  if (texture) {
    if (!SDL_SetRenderTarget(renderer, texture)) {
      SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed to set render target: %s",
                   SDL_GetError());
      return;
    }
  }
  for (auto &[_, layer] : _layers) {
    layer->draw(renderer);
  }
  if (texture) {
    SDL_SetRenderTarget(renderer, NULL);
  }
}