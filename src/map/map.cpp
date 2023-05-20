#include "map.h"

void Map::load(const char *name) {
  // TODO: implement

  LoggerManager::log_info("Loading map: " + std::string(name));
}

void Map::render(SDL_Renderer *renderer) {
  if (renderer == nullptr) {
    LoggerManager::log_error("SDL_Renderer is null");
    std::cerr << "SDL_Renderer is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  for (auto &[name, layer] : _config->layers) {
    layer.render(renderer);
  }
}
