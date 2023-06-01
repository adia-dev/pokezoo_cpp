#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <algorithm>
#include <assert.h>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <structs/my_symver.h>
#include <structs/my_vector.h>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "enums.h"

#define DEFAULT_WINDOW_WIDTH 1440
#define DEFAULT_WINDOW_HEIGHT 960
#define DEFAULT_WINDOW_FLAGS                                                   \
  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED |             \
      SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_OPENGL

#define DEFAULT_TILE_SIZE 16

struct WindowConfig {

  WindowConfig(const char *title, int width, int height, uint32_t flags)
      : title(title), width(width), height(height), flags(flags) {}

  const char *title;
  int width;
  int height;
  Vector2f scale = {1.0f, 1.0f};
  int tile_size = DEFAULT_TILE_SIZE;
  uint16_t max_fps = 240;
  uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS |
                   SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED |
                   SDL_WINDOW_OPENGL;
};

struct ApplicationConfig {

  ApplicationConfig(const char *title, int width, int height, uint32_t flags)
      : window_config(title, width, height, flags) {}

  WindowConfig window_config;
  Version version = {0, 0, 1};

  inline static bool is_debug_mode = false;

  inline static std::string font_path = "../src/assets/fonts/";
  inline static std::string map_path = "../src/assets/maps/";
  inline static std::string texture_path = "../src/assets/textures/";
  inline static std::string tileset_path = "../src/assets/tilesets/";
  inline static std::string animation_path = "../src/assets/animations/";
  inline static std::string audio_path = "../src/assets/audios/";

  static std::string_view get_asset_path(AssetDirectory dir) {
    switch (dir) {
    case AssetDirectory::TEXTURES:
      return texture_path;
    case AssetDirectory::FONTS:
      return font_path;
    case AssetDirectory::MAPS:
      return map_path;
    case AssetDirectory::ANIMATIONS:
      return animation_path;
    case AssetDirectory::AUDIO:
      return audio_path;
    default:
      return "";
    }
  }
};
