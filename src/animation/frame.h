#pragma once

#include <core/config.h>
#include <functional>

// struct for animation frame
struct Frame {
  SDL_Rect rect;
  int duration;
  bool flipped;

  // optional callback function
  std::function<void()> callback = nullptr;

  Frame()
      : rect({0, 0, 0, 0}), duration(0), flipped(false), callback(nullptr) {}

  Frame(const SDL_Rect &rect, int duration, bool flipped = false,
        std::function<void()> callback = nullptr)
      : rect(rect), duration(duration), flipped(flipped), callback(callback) {}
};