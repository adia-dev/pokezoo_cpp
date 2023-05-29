#include "trainer.h"
#include <managers/input/input_manager.h>
#include <utils/enums_utils.h>
#include <utils/math_utils.h>
#include <utils/string_utils.h>

Trainer::Trainer(const char *texture_name, int x, int y, int width, int height,
                 float scale)
    : Sprite(texture_name, x, y, width, height, scale) {}

void Trainer::render(SDL_Renderer *renderer) {
  // Custom rendering logic for the Trainer class
  // Example: Render additional Trainer-specific elements
  // ...

  // Call the base class render function to render the Sprite
  Sprite::render(renderer);
}

void Trainer::update(double delta_time) {
  // Custom update logic for the Trainer class
  // Example: Update Trainer-specific behavior
  // ...

  _is_running = InputManager::is_key_down(SDLK_LSHIFT);

  _desired_speed = (_is_running ? _run_speed : _walk_speed) *
                   InputManager::get_directional_input().magnitude();

  _speed = MathUtils::lerp(_speed, _desired_speed, 0.01f);

  Vector2i directional_input = InputManager::get_directional_input();
  Direction direction = Vector2i::direction_from_vector(directional_input);

  std::string animation_name = "idle";

  if (direction != Direction::NONE) {
    set_direction(direction);
  }

  if (directional_input.magnitude() > 0) {
    animation_name = _is_running ? "run" : "walk";
  }

  if (_riding_bike) {
    animation_name = "bike";
  }

  animation_name +=
      "_" + StringUtils::to_lower(EnumsUtils::direction_to_string(_direction));

  _animation_controller.play_animation(animation_name);

  _dest_rect.x += directional_input.x * _speed * delta_time;
  _dest_rect.y += directional_input.y * _speed * delta_time;

  // Call the base class update function to update the Sprite
  Sprite::update(delta_time);
}
