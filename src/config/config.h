#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <structs/my_symver.h>
#include <vector>

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480
#define DEFAULT_WINDOW_FLAGS SDL_WINDOW_SHOWN

#define DEFAULT_TILE_SIZE 32

struct WindowConfig {

  WindowConfig(const char *title, int width, int height, uint32_t flags)
      : title(title), width(width), height(height), flags(flags) {}

  const char *title;
  int width;
  int height;
  uint32_t flags;
};

struct ApplicationConfig {

  ApplicationConfig(const char *title, int width, int height, uint32_t flags)
      : window_config(title, width, height, flags) {}

  WindowConfig window_config;
  Version version = {0, 0, 1};

  const char *resource_path = "resources/";
  const char *map_path = "maps/";
  const char *tileset_path = "tilesets/";
  const char *font_path = "fonts/";
  const char *audio_path = "audio/";
};