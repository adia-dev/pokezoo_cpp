#include "sprite.h"
#include <managers/asset/asset_manager.h>
#include <managers/input/input_manager.h>
#include <managers/logger/logger_manager.h>
#include <utils/render_utils.h>

Sprite::Sprite(const std::string &name, const char *texture_name, int x, int y,
               int width, int height, float scale) {
  _texture = AssetManager::get_texture(texture_name);
  init(name, x, y, width, height, scale);
}

Sprite::Sprite(const std::string &name, SDL_Texture *texture, int x, int y,
               int width, int height, float scale) {
  _texture = texture;
  init(name, x, y, width, height, scale);
}

void Sprite::init(const std::string &name, int x, int y, int width, int height,
                  float scale) {
  _name = name;

  _src_rect.x = 0;
  _src_rect.y = 0;
  _src_rect.w = width;
  _src_rect.h = height;
  _dest_rect.x = x;
  _dest_rect.y = y;
  _dest_rect.w = width * scale;
  _dest_rect.h = height * scale;

  _id = UUID::generate_v4_ish();

  _debug |= ApplicationConfig::is_debug_mode;
}

void Sprite::render(SDL_Renderer *renderer) {
  if (_texture == nullptr) {
    LoggerManager::log_error("Could not render sprite, texture is null");
    return;
  }

  if (renderer == nullptr) {
    LoggerManager::log_error("Could not render sprite, renderer is null");
    return;
  }

  bool flip = false;

  if (!_animation_controller.get_current_animation().empty()) {
    _src_rect = _animation_controller.get_current_frame().rect;
    flip = _animation_controller.get_current_frame().flipped;
  }

  SDL_RenderCopyEx(renderer, _texture, &_src_rect, &_dest_rect, 0, nullptr,
                   flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void Sprite::update(double delta_time) {
  _animation_controller.update(delta_time);
}
