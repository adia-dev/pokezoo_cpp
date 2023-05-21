#pragma once

#include "sprite.h"

class Trainer : public Sprite {
public:
  Trainer(const char *texture_name, int x, int y, int width, int height,
          float scale = 1);

  void render(SDL_Renderer *renderer) override;
  void update(double delta_time) override;

  void set_name(const std::string &name) { _name = name; }
  const std::string &get_name() const { return _name; }

private:
  std::string _name;
  float _walk_speed = 300;
  float _run_speed = 500;
  float _speed = _walk_speed;
  float _desired_speed = _walk_speed;
  bool _is_running = false;
  bool _riding_bike = false;
};
