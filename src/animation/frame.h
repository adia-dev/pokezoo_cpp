#pragma once

#include <config/config.h>
#include <functional>

// struct for animation frame
struct Frame {
  SDL_Rect rect;
  int duration;
  bool flipped;

  // optional callback function
  std::function<void()> callback;

  Frame()
      : rect({0, 0, 0, 0}), duration(0), flipped(false), callback(nullptr) {}

  Frame(const SDL_Rect &rect, int duration, bool flipped = false,
        std::function<void()> callback = nullptr)
      : rect(rect), duration(duration), flipped(flipped), callback(callback) {}
};