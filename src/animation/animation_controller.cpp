#include "animation_controller.h"
#include <managers/logger/logger_manager.h>

AnimationController::AnimationController()
    : _current_frame_index(0), _current_frame_count(0) {}

void AnimationController::add_animation(const std::string &name,
                                        const Animation &animation) {
  _animations[name] = animation;
}

void AnimationController::play_animation(const std::string &name) {
  if (name == _current_animation) {
    return;
  }

  _current_animation = name;
  _current_frame_index = 0;
  _current_frame_count = 0;
}

Frame AnimationController::get_current_frame() const {
  const Animation &animation = _animations.at(_current_animation);
  return animation.frames[_current_frame_index];
}

void AnimationController::update(float delta_time) {
  if (!_is_playing || _current_animation == "" ||
      _animations.find(_current_animation) == _animations.end()) {
    return;
  }

  const Animation &animation = _animations.at(_current_animation);
  _current_frame_count += delta_time;

  if (_current_frame_count >= animation.frames[_current_frame_index].duration) {
    _current_frame_count = 0;
    _current_frame_index++;

    if (animation.frames[_current_frame_index].callback != nullptr) {
      animation.frames[_current_frame_index].callback();
    }

    switch (animation.direction) {
    case AnimationDirection::FORWARD:
      if (_current_frame_index >= animation.frames.size()) {
        _current_frame_index = animation.frames.size() - 1;
      }
      break;
    case AnimationDirection::REVERSE:
      if (_current_frame_index >= animation.frames.size()) {
        _current_frame_index = animation.frames.size() - 1;
      }
      break;
    case AnimationDirection::LOOP:
      if (_current_frame_index >= animation.frames.size()) {
        _current_frame_index = 0;
      }
      break;
    case AnimationDirection::ONCE:
      if (_current_frame_index >= animation.frames.size()) {
        _current_frame_index = 0;
      }
      break;
    case AnimationDirection::PINGPONG:
      if (_current_frame_index >= animation.frames.size()) {
        _current_frame_index =
            animation.frames.size() - 1; // not working, useless anyway
      }
      break;
    default:
      break;
    }
  }
}

void AnimationController::load_from_file(const std::string &file_path,
                                         SupportedSerializer serializer) {
  switch (serializer) {
  case SupportedSerializer::JSON:
    break;
  case SupportedSerializer::XML:
    break;
  default:
    LoggerManager::log_error("Attempted to load animation from file with an "
                             "unsupported serializer.");
    break;
  }
}
