#include "input_manager.h"

void InputManager::update_key_states() {
  auto &key_states = get()._key_states;

  for (auto &[code, state] : key_states) {
    switch (state) {
    case InputState::PRESSED:
      state = InputState::DOWN;
      break;
    case InputState::RELEASED:
      state = InputState::NOT_PRESSED;
      break;
    default:
      break;
    }
  }

  // update key direction, cannot go diagonally
  Vector2f desired_direction = Vector2f::zero();
  if (is_key_down(SDLK_w) || is_key_down(SDLK_UP)) {
    desired_direction.x = 0;
    desired_direction.y -= 1;
  }
  if (is_key_down(SDLK_s) || is_key_down(SDLK_DOWN)) {
    desired_direction.x = 0;
    desired_direction.y += 1;
  }
  if (is_key_down(SDLK_a) || is_key_down(SDLK_LEFT)) {
    desired_direction.y = 0;
    desired_direction.x -= 1;
  }
  if (is_key_down(SDLK_d) || is_key_down(SDLK_RIGHT)) {
    desired_direction.y = 0;
    desired_direction.x += 1;
  }
  desired_direction = desired_direction.normalized();
  get()._key_direction = get()._key_direction.lerp(desired_direction, 0.2f);
  if (get()._key_direction.magnitude() < 0.1f) {
    get()._key_direction = Vector2f::zero();
  }
}

std::map<SDL_Keycode, InputState> InputManager::get_key_states() {
  return get()._key_states;
}

Vector2f InputManager::get_directional_input() { return get()._key_direction; }

void InputManager::set_key_state(const SDL_Keycode &code,
                                 InputState new_state) {
  get()._key_states[code] = new_state;
}

bool InputManager::is_key_pressed(const SDL_Keycode &code) {
  return get()._key_states[code] == InputState::PRESSED;
}

bool InputManager::is_key_down(const SDL_Keycode &code) {
  return get()._key_states[code] == InputState::DOWN;
}

bool InputManager::is_key_released(const SDL_Keycode &code) {
  return get()._key_states[code] == InputState::RELEASED;
}

std::string InputManager::input_state_to_string(InputState state) {
  static std::map<InputState, std::string> input_state_map;

  if (input_state_map.empty()) {
    input_state_map[InputState::NOT_PRESSED] = "NOT PRESSED";
    input_state_map[InputState::PRESSED] = "PRESSED";
    input_state_map[InputState::DOWN] = "DOWN";
    input_state_map[InputState::RELEASED] = "RELEASED";
  }

  if (!input_state_map.count(state)) {
    return "UNKNOWN";
  }

  return input_state_map[state];
}

std::string InputManager::key_code_to_string(int code) {
  static std::map<int, std::string> key_code_to_string_map;
  if (key_code_to_string_map.empty()) {
    key_code_to_string_map[0] = "UNKNOWN";
    key_code_to_string_map['\r'] = "RETURN";
    key_code_to_string_map['\x1B'] = "ESCAPE";
    key_code_to_string_map['\b'] = "BACKSPACE";
    key_code_to_string_map['\t'] = "TAB";
    key_code_to_string_map[' '] = "SPACE";
    key_code_to_string_map['!'] = "EXCLAIM";
    key_code_to_string_map['"'] = "QUOTEDBL";
    key_code_to_string_map['#'] = "HASH";
    key_code_to_string_map['%'] = "PERCENT";
    key_code_to_string_map['$'] = "DOLLAR";
    key_code_to_string_map['&'] = "AMPERSAND";
    key_code_to_string_map['\''] = "QUOTE";
    key_code_to_string_map['('] = "LEFTPAREN";
    key_code_to_string_map[')'] = "RIGHTPAREN";
    key_code_to_string_map['*'] = "ASTERISK";
    key_code_to_string_map['+'] = "PLUS";
    key_code_to_string_map[','] = "COMMA";
    key_code_to_string_map['-'] = "MINUS";
    key_code_to_string_map['.'] = "PERIOD";
    key_code_to_string_map['/'] = "SLASH";
    key_code_to_string_map['0'] = "0";
    key_code_to_string_map['1'] = "1";
    key_code_to_string_map['2'] = "2";
    key_code_to_string_map['3'] = "3";
    key_code_to_string_map['4'] = "4";
    key_code_to_string_map['5'] = "5";
    key_code_to_string_map['6'] = "6";
    key_code_to_string_map['7'] = "7";
    key_code_to_string_map['8'] = "8";
    key_code_to_string_map['9'] = "9";
    key_code_to_string_map[':'] = "COLON";
    key_code_to_string_map[';'] = "SEMICOLON";
    key_code_to_string_map['<'] = "LESS";
    key_code_to_string_map['='] = "EQUALS";
    key_code_to_string_map['>'] = "GREATER";
    key_code_to_string_map['?'] = "QUESTION";
    key_code_to_string_map['@'] = "AT";
    key_code_to_string_map['['] = "LEFTBRACKET";
    key_code_to_string_map['\\'] = "BACKSLASH";
    key_code_to_string_map[']'] = "RIGHTBRACKET";
    key_code_to_string_map['^'] = "CARET";
    key_code_to_string_map['_'] = "UNDERSCORE";
    key_code_to_string_map['`'] = "BACKQUOTE";
    key_code_to_string_map['a'] = "a";
    key_code_to_string_map['b'] = "b";
    key_code_to_string_map['c'] = "c";
    key_code_to_string_map['d'] = "d";
    key_code_to_string_map['e'] = "e";
    key_code_to_string_map['f'] = "f";
    key_code_to_string_map['g'] = "g";
    key_code_to_string_map['h'] = "h";
    key_code_to_string_map['i'] = "i";
    key_code_to_string_map['j'] = "j";
    key_code_to_string_map['k'] = "k";
    key_code_to_string_map['l'] = "l";
    key_code_to_string_map['m'] = "m";
    key_code_to_string_map['n'] = "n";
    key_code_to_string_map['o'] = "o";
    key_code_to_string_map['p'] = "p";
    key_code_to_string_map['q'] = "q";
    key_code_to_string_map['r'] = "r";
    key_code_to_string_map['s'] = "s";
    key_code_to_string_map['t'] = "t";
    key_code_to_string_map['u'] = "u";
    key_code_to_string_map['v'] = "v";
    key_code_to_string_map['w'] = "w";
    key_code_to_string_map['x'] = "x";
    key_code_to_string_map['y'] = "y";
    key_code_to_string_map['z'] = "z";
    key_code_to_string_map[SDLK_CAPSLOCK] = "CAPSLOCK";
    key_code_to_string_map[SDLK_F1] = "F1";
    key_code_to_string_map[SDLK_F2] = "F2";
    key_code_to_string_map[SDLK_F3] = "F3";
    key_code_to_string_map[SDLK_F4] = "F4";
    key_code_to_string_map[SDLK_F5] = "F5";
    key_code_to_string_map[SDLK_F6] = "F6";
    key_code_to_string_map[SDLK_F7] = "F7";
    key_code_to_string_map[SDLK_F8] = "F8";
    key_code_to_string_map[SDLK_F9] = "F9";
    key_code_to_string_map[SDLK_F10] = "F10";
    key_code_to_string_map[SDLK_F11] = "F11";
    key_code_to_string_map[SDLK_F12] = "F12";
    key_code_to_string_map[SDLK_PRINTSCREEN] = "PRINTSCREEN";
    key_code_to_string_map[SDLK_SCROLLLOCK] = "SCROLLLOCK";
    key_code_to_string_map[SDLK_PAUSE] = "PAUSE";
    key_code_to_string_map[SDLK_INSERT] = "INSERT";
    key_code_to_string_map[SDLK_HOME] = "HOME";
    key_code_to_string_map[SDLK_PAGEUP] = "PAGEUP";

    key_code_to_string_map[SDLK_END] = "END";
    key_code_to_string_map[SDLK_PAGEDOWN] = "PAGEDOWN";
    key_code_to_string_map[SDLK_RIGHT] = "RIGHT";
    key_code_to_string_map[SDLK_LEFT] = "LEFT";
    key_code_to_string_map[SDLK_DOWN] = "DOWN";
    key_code_to_string_map[SDLK_UP] = "UP";
    key_code_to_string_map[SDLK_NUMLOCKCLEAR] = "NUMLOCKCLEAR";
    key_code_to_string_map[SDLK_KP_DIVIDE] = "KP_DIVIDE";
    key_code_to_string_map[SDLK_KP_MULTIPLY] = "KP_MULTIPLY";
    key_code_to_string_map[SDLK_KP_MINUS] = "KP_MINUS";
    key_code_to_string_map[SDLK_KP_PLUS] = "KP_PLUS";
    key_code_to_string_map[SDLK_KP_ENTER] = "KP_ENTER";
    key_code_to_string_map[SDLK_KP_1] = "KP_1";
    key_code_to_string_map[SDLK_KP_2] = "KP_2";
    key_code_to_string_map[SDLK_KP_3] = "KP_3";
    key_code_to_string_map[SDLK_KP_4] = "KP_4";
    key_code_to_string_map[SDLK_KP_5] = "KP_5";
    key_code_to_string_map[SDLK_KP_6] = "KP_6";
    key_code_to_string_map[SDLK_KP_7] = "KP_7";
    key_code_to_string_map[SDLK_KP_8] = "KP_8";
    key_code_to_string_map[SDLK_KP_9] = "KP_9";
    key_code_to_string_map[SDLK_KP_0] = "KP_0";
    key_code_to_string_map[SDLK_KP_PERIOD] = "KP_PERIOD";
    key_code_to_string_map[SDLK_APPLICATION] = "APPLICATION";
    key_code_to_string_map[SDLK_POWER] = "POWER";
    key_code_to_string_map[SDLK_KP_EQUALS] = "KP_EQUALS";
    key_code_to_string_map[SDLK_F13] = "F13";
    key_code_to_string_map[SDLK_F14] = "F14";
    key_code_to_string_map[SDLK_F15] = "F15";
    key_code_to_string_map[SDLK_F16] = "F16";
    key_code_to_string_map[SDLK_F17] = "F17";
    key_code_to_string_map[SDLK_F18] = "F18";
    key_code_to_string_map[SDLK_F19] = "F19";
    key_code_to_string_map[SDLK_F20] = "F20";
    key_code_to_string_map[SDLK_F21] = "F21";
    key_code_to_string_map[SDLK_F22] = "F22";
    key_code_to_string_map[SDLK_F23] = "F23";
    key_code_to_string_map[SDLK_F24] = "F24";
    key_code_to_string_map[SDLK_EXECUTE] = "EXECUTE";
    key_code_to_string_map[SDLK_HELP] = "HELP";
    key_code_to_string_map[SDLK_MENU] = "MENU";
    key_code_to_string_map[SDLK_SELECT] = "SELECT";
    key_code_to_string_map[SDLK_STOP] = "STOP";
    key_code_to_string_map[SDLK_AGAIN] = "AGAIN";
    key_code_to_string_map[SDLK_UNDO] = "UNDO";
    key_code_to_string_map[SDLK_CUT] = "CUT";
    key_code_to_string_map[SDLK_COPY] = "COPY";
    key_code_to_string_map[SDLK_PASTE] = "PASTE";
    key_code_to_string_map[SDLK_FIND] = "FIND";
    key_code_to_string_map[SDLK_MUTE] = "MUTE";
    key_code_to_string_map[SDLK_VOLUMEUP] = "VOLUMEUP";
    key_code_to_string_map[SDLK_VOLUMEDOWN] = "VOLUMEDOWN";
    key_code_to_string_map[SDLK_KP_COMMA] = "KP_COMMA";
    key_code_to_string_map[SDLK_KP_EQUALSAS400] = "KP_EQUALSAS400";
    key_code_to_string_map[SDLK_ALTERASE] = "ALTERASE";
    key_code_to_string_map[SDLK_SYSREQ] = "SYSREQ";
    key_code_to_string_map[SDLK_CANCEL] = "CANCEL";
    key_code_to_string_map[SDLK_CLEAR] = "CLEAR";
    key_code_to_string_map[SDLK_PRIOR] = "PRIOR";
    key_code_to_string_map[SDLK_RETURN2] = "RETURN2";
    key_code_to_string_map[SDLK_SEPARATOR] = "SEPARATOR";
    key_code_to_string_map[SDLK_OUT] = "OUT";
    key_code_to_string_map[SDLK_OPER] = "OPER";
    key_code_to_string_map[SDLK_CLEARAGAIN] = "CLEARAGAIN";
    key_code_to_string_map[SDLK_CRSEL] = "CRSEL";
    key_code_to_string_map[SDLK_EXSEL] = "EXSEL";
    key_code_to_string_map[SDLK_KP_00] = "KP_00";
    key_code_to_string_map[SDLK_KP_000] = "KP_000";
    key_code_to_string_map[SDLK_THOUSANDSSEPARATOR] = "THOUSANDSSEPARATOR";
    key_code_to_string_map[SDLK_DECIMALSEPARATOR] = "DECIMALSEPARATOR";
    key_code_to_string_map[SDLK_CURRENCYUNIT] = "CURRENCYUNIT";
    key_code_to_string_map[SDLK_CURRENCYSUBUNIT] = "CURRENCYSUBUNIT";
    key_code_to_string_map[SDLK_KP_LEFTPAREN] = "KP_LEFTPAREN";
    key_code_to_string_map[SDLK_KP_RIGHTPAREN] = "KP_RIGHTPAREN";
    key_code_to_string_map[SDLK_KP_LEFTBRACE] = "KP_LEFTBRACE";
    key_code_to_string_map[SDLK_KP_RIGHTBRACE] = "KP_RIGHTBRACE";
    key_code_to_string_map[SDLK_KP_TAB] = "KP_TAB";
    key_code_to_string_map[SDLK_KP_BACKSPACE] = "KP_BACKSPACE";
    key_code_to_string_map[SDLK_KP_A] = "KP_A";
    key_code_to_string_map[SDLK_KP_B] = "KP_B";
    key_code_to_string_map[SDLK_KP_C] = "KP_C";
    key_code_to_string_map[SDLK_KP_D] = "KP_D";
    key_code_to_string_map[SDLK_KP_E] = "KP_E";
    key_code_to_string_map[SDLK_KP_F] = "KP_F";
    key_code_to_string_map[SDLK_KP_XOR] = "KP_XOR";
    key_code_to_string_map[SDLK_KP_POWER] = "KP_POWER";
    key_code_to_string_map[SDLK_KP_PERCENT] = "KP_PERCENT";
    key_code_to_string_map[SDLK_KP_LESS] = "KP_LESS";
    key_code_to_string_map[SDLK_KP_GREATER] = "KP_GREATER";
    key_code_to_string_map[SDLK_KP_AMPERSAND] = "KP_AMPERSAND";
    key_code_to_string_map[SDLK_KP_DBLAMPERSAND] = "KP_DBLAMPERSAND";
    key_code_to_string_map[SDLK_KP_VERTICALBAR] = "KP_VERTICALBAR";
    key_code_to_string_map[SDLK_KP_DBLVERTICALBAR] = "KP_DBLVERTICALBAR";
    key_code_to_string_map[SDLK_KP_COLON] = "KP_COLON";
    key_code_to_string_map[SDLK_KP_HASH] = "KP_HASH";
    key_code_to_string_map[SDLK_KP_SPACE] = "KP_SPACE";
    key_code_to_string_map[SDLK_KP_AT] = "KP_AT";
    key_code_to_string_map[SDLK_KP_EXCLAM] = "KP_EXCLAM";
    key_code_to_string_map[SDLK_KP_MEMSTORE] = "KP_MEMSTORE";
    key_code_to_string_map[SDLK_KP_MEMRECALL] = "KP_MEMRECALL";
    key_code_to_string_map[SDLK_KP_MEMCLEAR] = "KP_MEMCLEAR";
    key_code_to_string_map[SDLK_KP_MEMADD] = "KP_MEMADD";
    key_code_to_string_map[SDLK_KP_MEMSUBTRACT] = "KP_MEMSUBTRACT";
    key_code_to_string_map[SDLK_KP_MEMMULTIPLY] = "KP_MEMMULTIPLY";
    key_code_to_string_map[SDLK_KP_MEMDIVIDE] = "KP_MEMDIVIDE";
    key_code_to_string_map[SDLK_KP_PLUSMINUS] = "KP_PLUSMINUS";
    key_code_to_string_map[SDLK_KP_CLEAR] = "KP_CLEAR";
    key_code_to_string_map[SDLK_KP_CLEARENTRY] = "KP_CLEARENTRY";
    key_code_to_string_map[SDLK_KP_BINARY] = "KP_BINARY";
    key_code_to_string_map[SDLK_KP_OCTAL] = "KP_OCTAL";
    key_code_to_string_map[SDLK_KP_DECIMAL] = "KP_DECIMAL";
    key_code_to_string_map[SDLK_KP_HEXADECIMAL] = "KP_HEXADECIMAL";
    key_code_to_string_map[SDLK_LCTRL] = "LCTRL";
    key_code_to_string_map[SDLK_LSHIFT] = "LSHIFT";
    key_code_to_string_map[SDLK_LALT] = "LALT";
    key_code_to_string_map[SDLK_LGUI] = "LGUI";
    key_code_to_string_map[SDLK_RCTRL] = "RCTRL";
    key_code_to_string_map[SDLK_RSHIFT] = "RSHIFT";
    key_code_to_string_map[SDLK_RALT] = "RALT";
    key_code_to_string_map[SDLK_RGUI] = "RGUI";
    key_code_to_string_map[SDLK_MODE] = "MODE";
    key_code_to_string_map[SDLK_AUDIONEXT] = "AUDIONEXT";
    key_code_to_string_map[SDLK_AUDIOPREV] = "AUDIOPREV";
    key_code_to_string_map[SDLK_AUDIOSTOP] = "AUDIOSTOP";
    key_code_to_string_map[SDLK_AUDIOPLAY] = "AUDIOPLAY";
    key_code_to_string_map[SDLK_AUDIOMUTE] = "AUDIOMUTE";
    key_code_to_string_map[SDLK_MEDIASELECT] = "MEDIASELECT";
    key_code_to_string_map[SDLK_WWW] = "WWW";
    key_code_to_string_map[SDLK_MAIL] = "MAIL";
    key_code_to_string_map[SDLK_CALCULATOR] = "CALCULATOR";
    key_code_to_string_map[SDLK_COMPUTER] = "COMPUTER";
    key_code_to_string_map[SDLK_AC_SEARCH] = "AC_SEARCH";
    key_code_to_string_map[SDLK_AC_HOME] = "AC_HOME";
    key_code_to_string_map[SDLK_AC_BACK] = "AC_BACK";
    key_code_to_string_map[SDLK_AC_FORWARD] = "AC_FORWARD";
    key_code_to_string_map[SDLK_AC_STOP] = "AC_STOP";
    key_code_to_string_map[SDLK_AC_REFRESH] = "AC_REFRESH";
    key_code_to_string_map[SDLK_AC_BOOKMARKS] = "AC_BOOKMARKS";
    key_code_to_string_map[SDLK_BRIGHTNESSDOWN] = "BRIGHTNESSDOWN";
    key_code_to_string_map[SDLK_BRIGHTNESSUP] = "BRIGHTNESSUP";
    key_code_to_string_map[SDLK_DISPLAYSWITCH] = "DISPLAYSWITCH";
    key_code_to_string_map[SDLK_KBDILLUMTOGGLE] = "KBDILLUMTOGGLE";
    key_code_to_string_map[SDLK_KBDILLUMDOWN] = "KBDILLUMDOWN";
    key_code_to_string_map[SDLK_KBDILLUMUP] = "KBDILLUMUP";
    key_code_to_string_map[SDLK_EJECT] = "EJECT";
    key_code_to_string_map[SDLK_SLEEP] = "SLEEP";
    key_code_to_string_map[SDLK_APP1] = "APP1";
    key_code_to_string_map[SDLK_APP2] = "APP2";
    key_code_to_string_map[SDLK_AUDIOREWIND] = "AUDIOREWIND";
    key_code_to_string_map[SDLK_AUDIOFASTFORWARD] = "AUDIOFASTFORWARD";
    key_code_to_string_map[SDLK_SOFTLEFT] = "SOFTLEFT";
    key_code_to_string_map[SDLK_SOFTRIGHT] = "SOFTRIGHT";
    key_code_to_string_map[SDLK_CALL] = "CALL";
    key_code_to_string_map[SDLK_ENDCALL] = "ENDCALL";
  }

  return key_code_to_string_map[code];
}

std::string InputManager::direction_to_string(const Direction &direction) {
  switch (direction) {
  case Direction::UP:
    return "UP";
  case Direction::DOWN:
    return "DOWN";
  case Direction::LEFT:
    return "LEFT";
  case Direction::RIGHT:
    return "RIGHT";
  default:
    return "UNKNOWN";
  }
}

Vector2f InputManager::direction_to_vector(const Direction &direction) {
  switch (direction) {
  case Direction::UP:
    return Vector2f::up();
  case Direction::DOWN:
    return Vector2f::down();
  case Direction::LEFT:
    return Vector2f::left();
  case Direction::RIGHT:
    return Vector2f::right();
  default:
    return Vector2f::zero();
  }
}

void InputManager::update_mouse_states() {
  for (auto &[button, state] : get()._mouse_states) {
    switch (state) {
    case InputState::PRESSED:
      get()._mouse_states[button] = InputState::DOWN;
      break;
    case InputState::RELEASED:
      get()._mouse_states[button] = InputState::NOT_PRESSED;
      break;
    default:
      break;
    }
  }
}

std::map<MouseButton, InputState> InputManager::get_mouse_states() {
  return get()._mouse_states;
}

void InputManager::set_mouse_button_state(const MouseButton &button,
                                          InputState new_state) {
  get()._mouse_states[button] = new_state;
}

bool InputManager::is_mouse_pressed(const MouseButton &button) {
  return get()._mouse_states[button] == InputState::PRESSED;
}

bool InputManager::is_mouse_down(const MouseButton &button) {
  return get()._mouse_states[button] == InputState::DOWN;
}

bool InputManager::is_mouse_released(const MouseButton &button) {
  return get()._mouse_states[button] == InputState::RELEASED;
}

Vector2f InputManager::get_mouse_position() { return get()._mouse_position; }

Vector2i InputManager::get_mouse_coords(int cell_width, int cell_height) {
  return Vector2i(static_cast<int>(get()._mouse_position.x / cell_width),
                  static_cast<int>(get()._mouse_position.y / cell_height));
}

void InputManager::set_mouse_position(const Vector2f &position) {
  get()._last_mouse_position = get()._mouse_position;
  get()._mouse_position = position;
}

void InputManager::set_mouse_position(float x, float y) {
  get()._last_mouse_position = get()._mouse_position;
  get()._mouse_position = Vector2f(x, y);
}

Vector2f InputManager::get_mouse_delta() {
  return get()._mouse_position - get()._last_mouse_position;
}

Vector2f InputManager::get_mouse_wheel() { return get()._mouse_wheel; }

void InputManager::set_mouse_wheel(const Vector2f &wheel) {
  get()._last_mouse_wheel = get()._mouse_wheel;
  get()._mouse_wheel = wheel;
}

void InputManager::set_mouse_wheel(float x, float y) {
  get()._last_mouse_wheel = get()._mouse_wheel;
  get()._mouse_wheel = Vector2f(x, y);
}

Vector2f InputManager::get_mouse_wheel_delta() {
  return get()._mouse_wheel - get()._last_mouse_wheel;
}

std::string InputManager::mouse_button_to_string(MouseButton button) {
  switch (button) {
  case MouseButton::LEFT:
    return "LEFT MOUSE BUTTON";
  case MouseButton::MIDDLE:
    return "MIDDLE MOUSE BUTTON";
  case MouseButton::RIGHT:
    return "RIGHT MOUSE BUTTON";
  default:
    return "UNKNOWN";
  }
}

MouseButton InputManager::uint8_to_mouse_button(uint8_t button) {
  switch (button) {
  case SDL_BUTTON_LEFT:
    return MouseButton::LEFT;
  case SDL_BUTTON_MIDDLE:
    return MouseButton::MIDDLE;
  case SDL_BUTTON_RIGHT:
    return MouseButton::RIGHT;
  default:
    return MouseButton::UNKNOWN;
  }
}

Direction InputManager::vector_to_direction(const Vector2f &vector) {
  float x = vector.x;
  float y = vector.y;

  if (x == 0 && y == 0) {
    return Direction::NONE;
  }

  if (std::abs(x) > std::abs(y)) {
    if (x > 0) {
      return Direction::RIGHT;
    } else {
      return Direction::LEFT;
    }
  } else {
    if (y > 0) {
      return Direction::DOWN;
    } else {
      return Direction::UP;
    }
  }
}
