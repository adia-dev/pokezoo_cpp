#pragma once

#include "sprite.h"

class Trainer : public Sprite {
public:
  Trainer(const char *texture_name, int x, int y, int width, int height,
          float scale = 1);

  void render(SDL_Renderer *renderer) override;
  void update(double delta_time) override;

  void toggle_bike() { _riding_bike = !_riding_bike; }
  void mount_bike() { _riding_bike = true; }
  void dismount_bike() { _riding_bike = false; }

private:
  float _walk_speed = 200;
  float _run_speed = 300;
  float _speed = _walk_speed;
  float _desired_speed = _walk_speed;
  bool _is_running = false;
  bool _riding_bike = false;
};
