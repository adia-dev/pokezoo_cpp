#pragma once

#include <animation/animation_controller.h>
#include <core/config.h>
#include <core/enums.h>
#include <structs/my_vector.h>
#include <utils/uuid.h>

class Sprite {
public:
  Sprite() = default;
  Sprite(const char *texture_name, int x, int y, int width, int height,
         float scale = 1);
  Sprite(SDL_Texture *texture, int x, int y, int width, int height,
         float scale = 1);
  virtual ~Sprite() = default;

  virtual void render(SDL_Renderer *renderer);
  virtual void update(float delta_time);

  void set_direction(Direction direction) { _direction = direction; }

  void set_position(int x, int y) {
    _dest_rect.x = x;
    _dest_rect.y = y;
  }

  template <typename T> void set_position(const Vector2<T> &position) {
    _dest_rect.x = position.x;
    _dest_rect.y = position.y;
  }

  void set_size(int width, int height) {
    _dest_rect.w = width * _scale;
    _dest_rect.h = height * _scale;
  }

  template <typename T> void set_size(const Vector2<T> &size) {
    _dest_rect.w = size.x * _scale;
    _dest_rect.h = size.y * _scale;
  }

  void set_scale(float scale) {
    _scale = scale;
    _dest_rect.w = _src_rect.w * _scale;
    _dest_rect.h = _src_rect.h * _scale;
  }

  void set_source_rect(int x, int y, int width, int height) {
    _src_rect.x = x;
    _src_rect.y = y;
    _src_rect.w = width;
    _src_rect.h = height;
  }

  const std::string &get_id() const { return _id; }

  AnimationController &get_animation_controller() {
    return _animation_controller;
  }
  void attach_animation_controller(AnimationController &animation_controller) {
    _animation_controller = animation_controller;
  }

  friend std::ostream &operator<<(std::ostream &os, const Sprite &sprite) {
    // print the class like a json object, deconstruct the rects
    // pretty print it with correct spacing
    os << "{\n";
    os << "  \"id\": \"" << sprite._id << "\",\n";
    os << "  \"texture\": \"" << sprite._texture << "\",\n";
    os << "  \"src_rect\": {\n";
    os << "    \"x\": " << sprite._src_rect.x << ",\n";
    os << "    \"y\": " << sprite._src_rect.y << ",\n";
    os << "    \"w\": " << sprite._src_rect.w << ",\n";
    os << "    \"h\": " << sprite._src_rect.h << "\n";
    os << "  },\n";
    os << "  \"dest_rect\": {\n";
    os << "    \"x\": " << sprite._dest_rect.x << ",\n";
    os << "    \"y\": " << sprite._dest_rect.y << ",\n";
    os << "    \"w\": " << sprite._dest_rect.w << ",\n";
    os << "    \"h\": " << sprite._dest_rect.h << "\n";
    os << "  },\n";
    os << "  \"scale\": " << sprite._scale << "\n";
    os << "}\n";
    return os;
  }

protected:
  std::string _id;
  Direction _direction = Direction::DOWN;
  AnimationController _animation_controller;
  SDL_Texture *_texture = nullptr;
  SDL_Rect _src_rect;
  SDL_Rect _dest_rect;
  float _scale = 1;

  void init(int x, int y, int width, int height, float scale);
};