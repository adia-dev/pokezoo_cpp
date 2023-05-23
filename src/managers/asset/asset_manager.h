#pragma once

#include <core/config.h>
#include <core/enums.h>

class AssetManager {
public:
  AssetManager() = default;
  ~AssetManager() = default;

  static AssetManager *get() {
    static std::unique_ptr<AssetManager> instance =
        std::make_unique<AssetManager>();
    return instance.get();
  }

  void clean();

  static SDL_Texture *
  get_texture(const char *name,
              AssetDirectory directory = AssetDirectory::TEXTURES,
              SDL_Renderer *renderer = nullptr);
  static TTF_Font *get_font(const char *name, int size);

  friend std::ostream &operator<<(std::ostream &os, const AssetManager &am) {
    // print in a json like fashion
    os << "{\n";
    os << "  \"textures\": {\n";
    for (auto &[name, texture] : am._textures) {
      os << "    \"" << name << "\": \"" << texture << "\",\n";
    }
    os << "  },\n";
    os << "  \"fonts\": {\n";
    for (auto &[name, font] : am._fonts) {
      os << "    \"" << name << "\": \"" << font << "\",\n";
    }
    os << "  }\n";
    os << "}\n";

    return os;
  }

private:
  std::map<std::string, SDL_Texture *> _textures;
  std::map<std::string, TTF_Font *> _fonts;
};