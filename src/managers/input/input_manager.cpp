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
  return manager->_keys.count(key) > 0 && manager->_keys[key] == true;
}

bool InputManager::are_keys_down(const std::vector<SDL_KeyCode> &keys) {
  auto *manager = InputManager::get();
  for (auto key : keys) {
    if (manager->_keys.count(key) == 0 || manager->_keys[key] == false) {
      return false;
    }
  }

  return true;
}

bool InputManager::is_key_up(SDL_KeyCode key) {
  auto *manager = InputManager::get();
  return manager->_keys.count(key) == 0 || manager->_keys[key] == false;
}

bool InputManager::is_mouse_down(int button) {
  auto *manager = InputManager::get();
  return manager->_mouse_buttons.count(button) > 0 &&
         manager->_mouse_buttons[button] == true;
}

bool InputManager::is_mouse_up(int button) {
  auto *manager = InputManager::get();
  return manager->_mouse_buttons.count(button) == 0 ||
         manager->_mouse_buttons[button] == false;
}
