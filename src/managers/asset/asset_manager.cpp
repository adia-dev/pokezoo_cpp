#include "asset_manager.h"
#include <application/application.h>

void AssetManager::clean() {
  for (auto &[name, texture] : _textures) {
    SDL_DestroyTexture(texture);
  }

  for (auto &[name, font] : _fonts) {
    TTF_CloseFont(font);
  }

  _textures.clear();
  _fonts.clear();
}

SDL_Texture *AssetManager::get_texture(const char *name,
                                       AssetDirectory directory,
                                       SDL_Renderer *renderer) {
  auto &manager = *AssetManager::get();
  auto &textures = manager._textures;

  auto it = textures.find(name);

  if (it != textures.end()) {
    return it->second;
  }

  std::string path =
      ApplicationConfig::get_asset_path(directory).data() + std::string(name);

  SDL_Surface *surface = IMG_Load(path.c_str());

  if (surface == nullptr) {
    std::cerr << "SDL_Surface is null !\nIt is likely that the image file "
              << path << " does not exist or is not a valid image file"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (renderer == nullptr) {
    renderer = Application::get()->get_renderer();
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  if (texture == nullptr) {
    std::cerr << "SDL_Texture is null!\nIt is likely that the image file "
              << path << " does not exist or is not a valid image file"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  SDL_FreeSurface(surface);

  textures[name] = texture;

  return texture;
}

TTF_Font *AssetManager::get_font(const char *name, int size) {
  auto &manager = *AssetManager::get();
  auto &fonts = manager._fonts;

  std::string key = name + std::to_string(size);

  auto it = fonts.find(key);

  if (it != fonts.end()) {
    return it->second;
  }

  std::string path = ApplicationConfig::font_path + name;

  TTF_Font *font = TTF_OpenFont(path.c_str(), size);

  if (font == nullptr) {
    std::cerr << "TTF_Font is null" << std::endl;
    exit(EXIT_FAILURE);
  }

  fonts[key] = font;

  return font;
}
