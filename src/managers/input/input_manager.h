#pragma once

#include <application/application.h>
#include <core/config.h>
#include <structs/my_vector.h>
#include <utils/string_utils.h>

class InputManager {
public:
  InputManager(const InputManager &) = delete;

  InputManager() = default;
  ~InputManager() = default;

  static InputManager &get() {
    static InputManager instance;
    return instance;
  }

  static void update_key_states();
  static void handle_event(const SDL_Event &event) {
    switch (event.type) {
    case SDL_KEYDOWN:
      set_key_state(event.key.keysym.sym, InputState::PRESSED);
      break;
    case SDL_KEYUP:
      set_key_state(event.key.keysym.sym, InputState::RELEASED);
      break;
    case SDL_MOUSEMOTION: {
      if (SDL_GetWindowFlags(Application::get()->get_window()) &
          SDL_WINDOW_ALLOW_HIGHDPI) {
        set_mouse_position(
            event.motion.x / Application::get_config()->window_config.scale.x *
                2.f,
            event.motion.y / Application::get_config()->window_config.scale.y *
                2.f);
      } else {
        set_mouse_position(event.motion.x, event.motion.y);
      }
    } break;
    case SDL_MOUSEWHEEL:
      set_mouse_wheel(event.wheel.x, event.wheel.y);
      break;
    case SDL_MOUSEBUTTONDOWN:
      set_mouse_button_state(uint8_to_mouse_button(event.button.button),
                             InputState::PRESSED);
      break;
    case SDL_MOUSEBUTTONUP:
      set_mouse_button_state(uint8_to_mouse_button(event.button.button),
                             InputState::RELEASED);
      break;
    default:
      break;
    }
  }

  static std::map<SDL_Keycode, InputState> get_key_states();
  static Vector2f get_directional_input();
  static void set_key_state(const SDL_Keycode &code, InputState new_state);

  /**
   * Checks wheter a key is PRESSED, DOWN or RELEASED
   * You can refer to the InputState enum for more information
   * @param code The key code to check
   * @return true if the key is in the specified state
   * @return false if the key is not in the specified state
   */
  static bool is_key_pressed(const SDL_Keycode &code);
  static bool is_key_down(const SDL_Keycode &code);
  static bool is_key_released(const SDL_Keycode &code);

  static std::string input_state_to_string(InputState state);
  static std::string key_code_to_string(int code);
  static std::string direction_to_string(const Direction &direction);
  static Vector2f direction_to_vector(const Direction &direction);
  static std::string mouse_button_to_string(MouseButton button);
  static MouseButton uint8_to_mouse_button(uint8_t button);
  static Direction vector_to_direction(const Vector2f &vector);

  /**
   * Updates the mouse states
   * Go from PRESSED to DOWN if the mouse button is still pressed
   * Go from RELEASED to NOT_PRESSED if the mouse button is not pressed anymore
   */
  static void update_mouse_states();

  static std::map<MouseButton, InputState> get_mouse_states();
  static void set_mouse_button_state(const MouseButton &button,
                                     InputState new_state);

  /**
   * Checks wheter a mouse button is in a specific state
   * Refer to the InputState enum for more information
   * @param button The mouse button to check
   * @return true if the mouse button is in the specified state
   * @return false if the mouse button is not in the specified state
   */
  static bool is_mouse_pressed(const MouseButton &button);
  static bool is_mouse_down(const MouseButton &button);
  static bool is_mouse_released(const MouseButton &button);

  static Vector2f get_mouse_position();
  static Vector2i get_mouse_coords(int cell_width = DEFAULT_TILE_SIZE,
                                   int cell_height = DEFAULT_TILE_SIZE);
  static void set_mouse_position(const Vector2f &position);
  static void set_mouse_position(float x, float y);

  static Vector2f get_mouse_delta();

  static Vector2f get_mouse_wheel();
  static void set_mouse_wheel(const Vector2f &wheel);
  static void set_mouse_wheel(float x, float y);

  static Vector2f get_mouse_wheel_delta();

  friend std::ostream &operator<<(std::ostream &os,
                                  const InputManager &InputManager) {
    os << "{\n";
    os << "  \"key_states\": {\n";
    for (auto it = InputManager._key_states.begin();
         it != InputManager._key_states.end(); ++it) {
      os << "    \"" << key_code_to_string(it->first) << "\": \""
         << input_state_to_string(it->second) << "\",\n";
    }
    os << "  },\n";
    os << "  \"key_direction\": {\n";
    os << "    \"x\": " << InputManager._key_direction.x << ",\n";
    os << "    \"y\": " << InputManager._key_direction.y << "\n";
    os << "  },\n";
    os << "  \"mouse_states\": {\n";
    for (auto it = InputManager._mouse_states.begin();
         it != InputManager._mouse_states.end(); ++it) {
      os << "    \"" << mouse_button_to_string(it->first) << "\": \""
         << input_state_to_string(it->second) << "\",\n";
    }
    os << "  },\n";
    os << "  \"mouse_position\": {\n";
    os << "    \"x\": " << InputManager._mouse_position.x << ",\n";
    os << "    \"y\": " << InputManager._mouse_position.y << "\n";
    os << "  },\n";
    os << "  \"last_mouse_position\": {\n";
    os << "    \"x\": " << InputManager._last_mouse_position.x << ",\n";
    os << "    \"y\": " << InputManager._last_mouse_position.y << "\n";
    os << "  },\n";
    os << "  \"mouse_coords\": {\n";
    os << "    \"x\": " << InputManager.get_mouse_coords().x << ",\n";
    os << "    \"y\": " << InputManager.get_mouse_coords().y << "\n";
    os << "  },\n";
    os << "  \"mouse_wheel\": {\n";
    os << "    \"x\": " << InputManager._mouse_wheel.x << ",\n";
    os << "    \"y\": " << InputManager._mouse_wheel.y << "\n";
    os << "  },\n";
    os << "  \"last_mouse_wheel\": {\n";
    os << "    \"x\": " << InputManager._last_mouse_wheel.x << ",\n";
    os << "    \"y\": " << InputManager._last_mouse_wheel.y << "\n";
    os << "  }\n";
    os << "}";
    return os;
  }

private:
  std::map<SDL_Keycode, InputState> _key_states;
  std::map<MouseButton, InputState> _mouse_states;
  Vector2f _key_direction = Vector2f(0, 0);

  Vector2f _mouse_position;
  Vector2f _last_mouse_position;
  Vector2f _mouse_delta;

  Vector2f _mouse_wheel;
  Vector2f _last_mouse_wheel;
  Vector2f _mouse_wheel_delta;
};
