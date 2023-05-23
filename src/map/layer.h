#pragma once

#include "tile.h"

struct Layer {

  Layer(const char *name, LayerType type, SDL_Texture *texture, int width,
        int height, int tile_size, std::vector<Tile> data)
      : name(name), type(type), texture(texture), width(width), height(height),
        tile_size(tile_size), data(data) {}

  const char *name;
  LayerType type;
  int width;
  int height;
  int tile_size;
  SDL_Texture *texture = nullptr;

  std::vector<Tile> data;

  void render(SDL_Renderer *renderer) {
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

    SDL_Rect src_rect;
    SDL_Rect dst_rect;

    src_rect.w = src_rect.h = dst_rect.w = dst_rect.h = tile_size;

    for (int row = 0; row < height; row++) {
      dst_rect.y = row * tile_size;

      for (int col = 0; col < width; col++) {
        dst_rect.x = col * tile_size;

        uint16_t tile_id = data[(row * width) + col].get_id();

        if (tile_id != 0) {
          src_rect.x = (tile_id - 1) * tile_size;
          SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
        }
      }
    }
  }
};
