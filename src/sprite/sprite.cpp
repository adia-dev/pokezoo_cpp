#include "sprite.h"
#include <managers/asset/asset_manager.h>
#include <managers/input/input_manager.h>
#include <managers/logger/logger_manager.h>
#include <utils/render_utils.h>

Sprite::Sprite(const char *texture_name, int x, int y, int width, int height,
               float scale) {
  _texture = AssetManager::get_texture(texture_name);
  init(x, y, width, height, scale);
}

Sprite::Sprite(SDL_Texture *texture, int x, int y, int width, int height,
               float scale) {
  _texture = texture;
  init(x, y, width, height, scale);
}

void Sprite::init(int x, int y, int width, int height, float scale) {
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

  if (!_animation_controller.get_current_animation().empty()) {
    _src_rect = _animation_controller.get_current_frame().rect;
  }

  SDL_RenderCopy(renderer, _texture, &_src_rect, &_dest_rect);

  if (_debug || ApplicationConfig::is_debug_mode) {

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &_dest_rect);

    if (contains(InputManager::get_mouse_position())) {
      std::stringstream ss;
      ss << "id: " << _id << '\n';
      ss << "name: " << _name << '\n';
      ss << "x: " << _dest_rect.x << " y: " << _dest_rect.y;
      RenderUtils::render_text(
          renderer, AssetManager::get_font("Poppins/Poppins-Regular.ttf", 8),
          ss.str().c_str(), {255, 255, 255, 255}, _dest_rect.x,
          _dest_rect.y - 16, true);
    }
  }
}

void Sprite::update(double delta_time) {
  _animation_controller.update(delta_time);
}