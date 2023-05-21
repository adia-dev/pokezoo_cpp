#pragma once

#include <core/config.h>
#include <structs/my_vector.h>

class InputManager {

public:
  InputManager() = default;
  ~InputManager() = default;

  static InputManager *get() {
    static std::unique_ptr<InputManager> instance =
        std::make_unique<InputManager>();
    return instance.get();
  }

  void clean();

  void handle_events(SDL_Event &event);

  static bool is_key_down(SDL_KeyCode key);
  static bool are_keys_down(const std::vector<SDL_KeyCode> &keys);
  static bool is_key_up(SDL_KeyCode key);

  static bool is_mouse_down(int button);
  static bool is_mouse_up(int button);

  static Vector2i get_mouse_position() { return get()->_mouse_position; }
  static Vector2i get_mouse_delta() { return get()->_mouse_delta; }

  friend std::ostream &operator<<(std::ostream &os, const InputManager &im) {
    // make it seem like it is a json file
    os << "{\n";
    os << "\t\"keys\": {\n";
    for (auto &key : im._keys) {
      os << "\t\t\"" << key.first << "\": " << key.second << ",\n";
    }
    os << "\t},\n";
    os << "\t\"mouse_buttons\": {\n";
    for (auto &button : im._mouse_buttons) {
      os << "\t\t\"" << button.first << "\": " << button.second << ",\n";
    }
    os << "\t},\n";
    os << "\t\"mouse_position\": {\n";
    os << "\t\t\"x\": " << im._mouse_position.x << ",\n";
    os << "\t\t\"y\": " << im._mouse_position.y << "\n";
    os << "\t},\n";
    os << "\t\"mouse_delta\": {\n";
    os << "\t\t\"x\": " << im._mouse_delta.x << ",\n";
    os << "\t\t\"y\": " << im._mouse_delta.y << "\n";
    os << "\t}\n";
    os << "}\n";
    return os;
  }

private:
  std::map<int, bool> _keys;
  std::map<int, bool> _mouse_buttons;
  Vector2i _mouse_position;
  Vector2i _mouse_delta;
};