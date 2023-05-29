#include "serializer.h"
#include <managers/logger/logger_manager.h>

void AnimationSerializer::load_animations(AnimationController &controller,
                                          const std::string &json_file_path,
                                          const std::string &key) {
  std::ifstream file(json_file_path);
  if (!file.is_open()) {
    LoggerManager::log_fatal("Could not open file " + json_file_path);
    return;
  }

  json json_data;
  try {
    file >> json_data;
  } catch (const std::exception &e) {
    LoggerManager::log_fatal("Could not parse JSON file " + json_file_path +
                             ": " + e.what());
    file.close();
    return;
  }

  file.close();

  if (!json_data.is_object()) {
    // Handle invalid JSON object error
    LoggerManager::log_fatal("Could not parse JSON file " + json_file_path +
                             ": invalid JSON object");
    return;
  }

  if (key.empty() && json_data.find("animations") == json_data.end()) {
    // Handle missing "animations" key error
    LoggerManager::log_fatal("Could not parse JSON file " + json_file_path +
                             ": missing \"animations\" key");
    return;
  } else if (!key.empty() && json_data.find(key) == json_data.end()) {
    // Handle missing key error
    LoggerManager::log_fatal("Could not parse JSON file " + json_file_path +
                             ": missing \"" + key + "\" key");
    return;
  }

  const json &animations_json =
      key.empty() ? json_data["animations"] : json_data[key]["animations"];

  if (!animations_json.is_array()) {
    // Handle invalid "animations" value error
    LoggerManager::log_fatal("Could not parse JSON file " + json_file_path +
                             ": invalid \"animations\" value");
    return;
  }

  for (const auto &animation_json : animations_json) {
    if (!animation_json.is_object()) {
      // Handle invalid animation JSON object error
      LoggerManager::log_warning("Could not parse JSON file " + json_file_path +
                                 ": invalid animation JSON object");
      continue;
    }

    if (animation_json.find("preset") != animation_json.end() &&
        animation_json["preset"].is_string()) {
      if (animation_json["preset"] == "walk") {
        walk_preset(controller, animation_json);
      }

      if (animation_json["preset"] == "idle_down") {
        idle_down_preset(controller, animation_json);
      }
      continue;
    }

    Animation new_animation;
    process_animation_json(animation_json, new_animation);
    controller.add_animation(new_animation.name, new_animation);
  }
}

void AnimationSerializer::process_animation_json(const json &animation_json,
                                                 Animation &animation) {
  if (animation_json.find("name") != animation_json.end() &&
      animation_json["name"].is_string()) {
    animation.name = animation_json["name"].get<std::string>();
  } else {
    // Handle missing or invalid "name" value error
    LoggerManager::log_warning("Could not parse animation JSON object: missing "
                               "or invalid \"name\" value");
    return;
  }

  if (animation_json.find("direction") != animation_json.end() &&
      animation_json["direction"].is_string()) {
    const std::string &direction =
        animation_json["direction"].get<std::string>();
    if (direction == "LOOP") {
      animation.direction = AnimationDirection::LOOP;
    } else if (direction == "FORWARD") {
      animation.direction = AnimationDirection::FORWARD;
    } else if (direction == "REVERSE") {
      animation.direction = AnimationDirection::REVERSE;
    } else {
      // Handle invalid "direction" value error
      LoggerManager::log_warning("Could not parse animation JSON object: "
                                 "invalid \"direction\" value");
      return;
    }
  } else {
    // Handle missing or invalid "direction" value error
    LoggerManager::log_warning("Could not parse animation JSON object: missing "
                               "or invalid \"direction\" value");
    return;
  }

  if (animation_json.find("frames") != animation_json.end() &&
      animation_json["frames"].is_array()) {
    const json &frames_json = animation_json["frames"];
    for (const auto &frame_json : frames_json) {
      if (!frame_json.is_object()) {
        // Handle invalid frame JSON object error
        LoggerManager::log_warning("Could not parse animation JSON object: "
                                   "invalid frame JSON object");
        continue;
      }

      Frame frame;
      process_frame_json(frame_json, frame);
      animation.frames.push_back(frame);
    }
  } else {
    // Handle missing or invalid "frames" value error
    LoggerManager::log_warning("Could not parse animation JSON object: missing "
                               "or invalid \"frames\" value");
    return;
  }
}

void AnimationSerializer::process_frame_json(const json &frame_json,
                                             Frame &frame) {
  if (frame_json.find("rect") != frame_json.end() &&
      frame_json["rect"].is_object()) {
    const json &rect_json = frame_json["rect"];
    if (rect_json.find("x") != rect_json.end() && rect_json["x"].is_number()) {
      frame.rect.x = rect_json["x"].get<int>();
    }
    if (rect_json.find("y") != rect_json.end() && rect_json["y"].is_number()) {
      frame.rect.y = rect_json["y"].get<int>();
    }
    if (rect_json.find("width") != rect_json.end() &&
        rect_json["width"].is_number()) {
      frame.rect.w = rect_json["width"].get<int>();
    }
    if (rect_json.find("height") != rect_json.end() &&
        rect_json["height"].is_number()) {
      frame.rect.h = rect_json["height"].get<int>();
    }
  } else {
    // Handle missing or invalid "rect" value error
    LoggerManager::log_warning("Could not parse frame JSON object: missing or "
                               "invalid \"rect\" value");
    return;
  }

  if (frame_json.find("duration") != frame_json.end() &&
      frame_json["duration"].is_number()) {
    frame.duration = frame_json["duration"].get<int>();
  } else {
    // Handle missing or invalid "duration" value error
    LoggerManager::log_warning("Could not parse frame JSON object: missing or "
                               "invalid \"duration\" value");
    return;
  }

  if (frame_json.find("flipped") != frame_json.end() &&
      frame_json["flipped"].is_boolean()) {
    frame.flipped = frame_json["flipped"].get<bool>();
  } else {
    // Handle missing or invalid "flipped" value error
    LoggerManager::log_warning("Could not parse frame JSON object: missing or "
                               "invalid \"flipped\" value");
    return;
  }
}

void AnimationSerializer::walk_preset(AnimationController &controller,
                                      const json &animation_json) {
  std::vector<std::string> directions = {"up", "down", "left", "right"};
  Vector2i start = {animation_json["start"]["x"], animation_json["start"]["y"]};
  Vector2i size = {animation_json["size"]["width"],
                   animation_json["size"]["height"]};
  for (int i = 0; i < directions.size(); i++) {
    Animation new_animation("walk_" + directions[i], AnimationDirection::LOOP);
    for (int j = 0; j < 3; j++) {

      if (j == 0) {
        Animation idle_animation("idle_" + directions[i],
                                 AnimationDirection::LOOP);
        Frame frame;
        frame.rect = {start.x + j * size.x, start.y + i * size.y, size.x,
                      size.y};
        frame.duration = 100;
        frame.flipped = false;
        idle_animation.frames.push_back(frame);
        controller.add_animation(idle_animation.name, idle_animation);
      }

      Frame frame;
      frame.rect = {start.x + j * size.x, start.y + i * size.y, size.x, size.y};
      frame.duration = 100;
      frame.flipped = false;
      new_animation.frames.push_back(frame);
      controller.add_animation(new_animation.name, new_animation);
    }
  }
}

void AnimationSerializer::idle_down_preset(AnimationController &controller,
                                           const json &animation_json) {
  Vector2i start = {animation_json["start"]["x"], animation_json["start"]["y"]};
  Vector2i size = {animation_json["size"]["width"],
                   animation_json["size"]["height"]};
  int count = animation_json["count"];
  Animation new_animation("idle_down", AnimationDirection::LOOP);

  Animation idle_animation("idle_down", AnimationDirection::LOOP);
  for (int i = 0; i < count; ++i) {
    Frame frame;
    frame.rect = {start.x + i * size.x, start.y, size.x, size.y};
    frame.duration = 100;
    frame.flipped = false;
    idle_animation.frames.push_back(frame);
  }
  controller.add_animation(idle_animation.name, idle_animation);
}
