#include "input_manager.h"

#include <managers/logger/logger_manager.h>

void InputManager::clean() {
  _keys.clear();
  _mouse_buttons.clear();
}

void InputManager::handle_events(SDL_Event &event) {
  switch (event.type) {
  case SDL_KEYDOWN:
    _keys[event.key.keysym.sym] = true;
    break;
  case SDL_KEYUP:
    _keys[event.key.keysym.sym] = false;
    break;
  case SDL_MOUSEBUTTONDOWN:
    _mouse_buttons[event.button.button] = true;
    break;
  case SDL_MOUSEBUTTONUP:
    _mouse_buttons[event.button.button] = false;
    break;
  case SDL_MOUSEMOTION:
    _mouse_position.x = event.motion.x;
    _mouse_position.y = event.motion.y;
    _mouse_delta.x = event.motion.xrel;
    _mouse_delta.y = event.motion.yrel;
    break;
  default:
    break;
  }
}

bool InputManager::is_key_down(SDL_KeyCode key) {
  auto *manager = InputManager::get();
  return manager->_keys[key];
}

bool InputManager::are_keys_down(const std::vector<SDL_KeyCode> &keys) {
  auto *manager = InputManager::get();
  for (auto key : keys) {
    if (!manager->_keys[key]) {
      return false;
    }
  }

  return true;
}

bool InputManager::is_key_up(SDL_KeyCode key) {
  auto *manager = InputManager::get();
  return !manager->_keys[key];
}

bool InputManager::is_mouse_down(int button) {
  auto *manager = InputManager::get();
  return manager->_mouse_buttons[button];
}

bool InputManager::is_mouse_up(int button) {
  auto *manager = InputManager::get();
  return !manager->_mouse_buttons[button];
}

Vector2i InputManager::get_directional_input() {
  // return the direction of the keys clamped, not normalized, can only go one
  // diirection at a time
  auto *manager = InputManager::get();
  Vector2i direction = Vector2i::zero();

  if (manager->_keys[SDLK_z] || manager->_keys[SDLK_UP]) {
    direction.y = -1;
  } else if (manager->_keys[SDLK_s] || manager->_keys[SDLK_DOWN]) {
    direction.y = 1;
  }

  if (manager->_keys[SDLK_a] || manager->_keys[SDLK_LEFT]) {
    direction.x = -1;
  } else if (manager->_keys[SDLK_d] || manager->_keys[SDLK_RIGHT]) {
    direction.x = 1;
  }

  return direction;
}
