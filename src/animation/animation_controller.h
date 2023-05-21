#pragma once

#include "animation.h"
#include <core/config.h>

class AnimationController {
public:
  AnimationController();

  void add_animation(const std::string &name, const Animation &animation);
  void play_animation(const std::string &name);
  Frame get_current_frame() const;

  void update(float delta_time);

  bool is_playing() const { return _is_playing; }
  void resume() { _is_playing = true; }
  void pause() { _is_playing = false; }

  void load_from_file(const std::string &file_path,
                      SupportedSerializer serializer);

  // getters and setters
  std::string get_current_animation() const { return _current_animation; }

  size_t get_current_frame_index() const { return _current_frame_index; }
  void set_current_frame_index(size_t current_frame_index) {
    _current_frame_index = current_frame_index;
  }

  int get_current_frame_count() const { return _current_frame_count; }

private:
  bool _is_playing;

  std::map<std::string, Animation> _animations;
  std::string _current_animation;
  size_t _current_frame_index;
  int _current_frame_count;
};
