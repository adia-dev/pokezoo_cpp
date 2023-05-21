#pragma once

#include "frame.h"
#include <config/config.h>
#include <config/enums.h>
#include <functional>

struct Animation {
  std::string name;
  std::vector<Frame> frames;
  Animation *next_animation;
  AnimationDirection direction;

  void add_frame(const Frame &frame) { frames.push_back(frame); }
  void set_next_animation(Animation *animation) { next_animation = animation; }
  void remove_frame(size_t index) {
    if (index < 0 || index >= frames.size()) {
      return;
    }
    frames.erase(frames.begin() + index);
  }

  Frame &get_frame(size_t index) {
    if (index < 0 || index >= frames.size()) {
      throw std::out_of_range("Could not get frame at index " +
                              std::to_string(index) + " from animation " +
                              name);
    }
    return frames[index];
  }

  Animation()
      : name(""), next_animation(nullptr), direction(AnimationDirection::LOOP) {
  }

  Animation(const std::string &name,
            AnimationDirection direction = AnimationDirection::LOOP)
      : name(name), next_animation(nullptr), direction(direction) {}
};
