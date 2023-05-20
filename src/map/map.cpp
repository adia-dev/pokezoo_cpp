#include "map.h"

void Map::load(const char *name) {
  // TODO: implement

  std::cout << "Loading map: " << name << std::endl;
}

void Map::render(SDL_Renderer *renderer) {
  if (renderer == nullptr) {
    std::cerr << "SDL_Renderer is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  for (auto &[name, layer] : _config->layers) {
    layer.render(renderer);
  }
}
