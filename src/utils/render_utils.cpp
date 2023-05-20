#include "render_utils.h"

namespace RenderUtils {

void render_texture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y,
                    int w, int h) {
  if (renderer == nullptr) {
    std::cerr << "SDL_Renderer is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (texture == nullptr) {
    std::cerr << "SDL_Texture is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  SDL_Rect src_rect;
  SDL_Rect dst_rect;

  src_rect.w = src_rect.h = dst_rect.w = dst_rect.h = 32;

  for (int row = 0; row < h; row++) {
    dst_rect.y = row * 32;

    for (int col = 0; col < w; col++) {
      dst_rect.x = col * 32;

      uint16_t tile_id = 1;

      if (tile_id != 0) {
        src_rect.x = (tile_id - 1) * 32;
        SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
      }
    }
  }
}

void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                 SDL_Color color, int x, int y) {
  if (renderer == nullptr) {
    std::cerr << "SDL_Renderer is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (font == nullptr) {
    std::cerr << "TTF_Font is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);

  if (surface == nullptr) {
    std::cerr << "SDL_Surface is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  if (texture == nullptr) {
    std::cerr << "SDL_Texture is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  SDL_Rect dst_rect;

  dst_rect.x = x;
  dst_rect.y = y;
  dst_rect.w = surface->w;
  dst_rect.h = surface->h;

  SDL_RenderCopy(renderer, texture, nullptr, &dst_rect);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

} // namespace RenderUtils