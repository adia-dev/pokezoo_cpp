#include "trainer.h"
#include <managers/input/input_manager.h>
#include <utils/enums_utils.h>
#include <utils/math_utils.h>
#include <utils/string_utils.h>

Trainer::Trainer(const std::string &name, const char *texture_name, int x,
                 int y, int width, int height, float scale)
    : Sprite(name, texture_name, x, y, width, height, scale) {}

void Trainer::render(SDL_Renderer *renderer) {
  // Custom rendering logic for the Trainer class
  // Example: Render additional Trainer-specific elements
  // ...

  // Call the base class render function to render the Sprite
  Sprite::render(renderer);
}

void Trainer::update(double delta_time) {
  // Call the base class update function to update the Sprite
  Sprite::update(delta_time);

  auto input_direction = InputManager::get_directional_input();

  if (InputManager::is_key_pressed(SDLK_b)) {
    toggle_bike();
  }

  // set the player's direction
  Direction player_direction =
      InputManager::vector_to_direction(input_direction);
  if (player_direction != Direction::NONE)
    set_direction(player_direction);

  std::string animation_prefix =
      input_direction.magnitude() > 0.1f
          ? InputManager::is_key_down(SDLK_LSHIFT) ? "run_" : "walk_"
          : "idle_";

  float speed = InputManager::is_key_down(SDLK_LSHIFT) ? 250.f : 150.f;

  if (_riding_bike) {
    if (animation_prefix == "idle_")
      animation_prefix = "bike_idle_";
    else
      animation_prefix = "bike_";

    speed = 500.f;
  }

  if (input_direction.magnitude() > 0.1f) {

    get_animation_controller().play_animation(
        animation_prefix +
        StringUtils::to_lower(
            InputManager::direction_to_string(player_direction)));

    move(input_direction.x * delta_time * speed,
         input_direction.y * delta_time * speed);
  } else {
    get_animation_controller().play_animation(
        animation_prefix +
        StringUtils::to_lower(InputManager::direction_to_string(_direction)));
  }
}
