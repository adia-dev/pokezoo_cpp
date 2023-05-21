#pragma once

#include <core/config.h>
#include <managers/logger/logger_manager.h>

enum LayerType { TILE_LAYER, OBJECT_LAYER, OBSTACLE_LAYER };

struct Layer {

  Layer(const char *name, LayerType type, SDL_Texture *texture, int width,
        int height, int tile_size, std::vector<uint16_t> data)
      : name(name), type(type), texture(texture), width(width), height(height),
        tile_size(tile_size), data(data) {}

  const char *name;
  LayerType type;
  int width;
  int height;
  int tile_size;
  SDL_Texture *texture = nullptr;

  std::vector<uint16_t> data;

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

        uint16_t tile_id = data[(row * width) + col];

        if (tile_id != 0) {
          src_rect.x = (tile_id - 1) * tile_size;
          SDL_RenderCopy(renderer, texture, &src_rect, &dst_rect);
        }
      }
    }
  }
};

struct MapConfig {

  MapConfig(const char *name, int width, int height, int tile_size)
      : name(name), width(width), height(height), tile_size(tile_size) {}

  const char *name;
  int width;
  int height;
  int tile_size;

  std::map<std::string, Layer> layers;
};

class Map {
public:
  Map() = default;
  ~Map() = default;

  static Map *get() {
    static std::unique_ptr<Map> instance = std::make_unique<Map>();
    return instance.get();
  }

  void load(const char *name);
  void render(SDL_Renderer *renderer);

  int get_width() const { return _config->width; }
  int get_height() const { return _config->height; }
  int get_tile_size() const { return _config->tile_size; }

  Layer *get_layer(const char *name) {
    if (!name)
      return NULL;
    if (_config->layers.find(name) == _config->layers.end())
      return NULL;
    return &_config->layers.at(name);
  }

private:
  std::unique_ptr<MapConfig> _config = nullptr;
};