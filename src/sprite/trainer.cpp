#include "trainer.h"
#include <managers/input/input_manager.h>

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

void Trainer::update(float delta_time) {
  // Custom update logic for the Trainer class
  // Example: Update Trainer-specific behavior
  // ...

  _is_running = InputManager::is_key_down(SDLK_LSHIFT);

  _speed = _is_running ? _run_speed : _walk_speed;

  if (InputManager::is_key_down(SDLK_z) || InputManager::is_key_down(SDLK_UP)) {
    _dest_rect.y -= _speed * delta_time;
  }

  else if (InputManager::is_key_down(SDLK_s) ||
           InputManager::is_key_down(SDLK_DOWN)) {
    _dest_rect.y += _speed * delta_time;
  }

  else if (InputManager::is_key_down(SDLK_q) ||
           InputManager::is_key_down(SDLK_LEFT)) {
    _dest_rect.x -= _speed * delta_time;
  }

  else if (InputManager::is_key_down(SDLK_d) ||
           InputManager::is_key_down(SDLK_RIGHT)) {
    _dest_rect.x += _speed * delta_time;
  }

  // Call the base class update function to update the Sprite
  Sprite::update(delta_time);
}
