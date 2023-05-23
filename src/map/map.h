#pragma once

#include "layer.h"

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