#include "render_utils.h"
#include <application/application.h>

namespace RenderUtils {

void render_texture(SDL_Renderer *renderer, SDL_Texture *texture,
                    SDL_Rect src_rect, SDL_Rect dst_rect, bool repeat) {
  if (renderer == nullptr) {
    LoggerManager::log_error("SDL_Renderer is null");
    std::cerr << "SDL_Renderer is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (texture == nullptr) {
    LoggerManager::log_error("SDL_Texture is null");
    std::cerr << "SDL_Texture is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  src_rect.w = src_rect.h = dst_rect.w = dst_rect.h = 32;

  if (repeat) {
    for (int row = 0; row < Application::get_config()->window_config.height;
         row++) {
      dst_rect.y = row * 32;

      for (int col = 0; col < Application::get_config()->window_config.width;
           col++) {
        dst_rect.x = col * 32;

        uint16_t tile_id = 1;

        if (tile_id != 0) {
          src_rect.x = (tile_id - 1) * 32;
          SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
        }
      }
    }
  } else {
    SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
  }
}

void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                 SDL_Color color, int x, int y, bool wrap) {
  if (renderer == nullptr) {
    LoggerManager::log_error("SDL_Renderer is null");
    std::cerr << "SDL_Renderer is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (font == nullptr) {
    LoggerManager::log_error("TTF_Font is null");
    std::cerr << "TTF_Font is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  SDL_Surface *surface = nullptr;
  if (wrap) {
    surface = TTF_RenderText_Blended_Wrapped(font, text, color, 200);
  } else {
    surface = TTF_RenderText_Solid(font, text, color);
  }

  if (surface == nullptr) {
    LoggerManager::log_error("SDL_Surface is null");
    std::cerr << "SDL_Surface is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  if (texture == nullptr) {
    LoggerManager::log_error("SDL_Texture is null");
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

void render_grid(SDL_Renderer *renderer, int width, int height, int tile_size,
                 const SDL_Color &color) {
  if (renderer == nullptr) {
    LoggerManager::log_fatal("SDL_Renderer is null");
    exit(EXIT_FAILURE);
  }

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

  int x = 0;
  int y = 0;
  for (int row = 0; row < height; row++) {
    SDL_RenderDrawLine(renderer, x, y + row * tile_size, x + width * tile_size,
                       y + row * tile_size);
  }

  for (int col = 0; col < width; col++) {
    SDL_RenderDrawLine(renderer, x + col * tile_size, y, x + col * tile_size,
                       y + height * tile_size);
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void render_rect(SDL_Renderer *renderer, SDL_Rect rect, const SDL_Color &color,
                 bool fill) {
  if (renderer == nullptr) {
    LoggerManager::log_fatal("SDL_Renderer is null");
    exit(EXIT_FAILURE);
  }

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

  if (fill)
    SDL_RenderFillRect(renderer, &rect);
  else
    SDL_RenderDrawRect(renderer, &rect);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void render_square(SDL_Renderer *renderer, const Vector2i &position, int size,
                   const SDL_Color &color, bool fill) {
  if (renderer == nullptr) {
    LoggerManager::log_fatal("SDL_Renderer is null");
    exit(EXIT_FAILURE);
  }

  SDL_Rect rect;

  rect.x = position.x;
  rect.y = position.y;
  rect.w = size;
  rect.h = size;

  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

  if (fill)
    SDL_RenderFillRect(renderer, &rect);
  else
    SDL_RenderDrawRect(renderer, &rect);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

} // namespace RenderUtils