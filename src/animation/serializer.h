#pragma once

#include <core/config.h>
#include <core/enums.h>
#include <fstream>
#include <parsers/json.hpp>

#include "animation.h"
#include "animation_controller.h"

using json = nlohmann::json;

namespace AnimationSerializer {
void load_animations(AnimationController &controller,
                     const std::string &json_file_path,
                     const std::string &key = "");
void process_animation_json(const json &animation_json, Animation &animation);
void process_frame_json(const json &frame_json, Frame &frame);

void walk_preset(AnimationController &controller, const json &animation_json);
void idle_down_preset(AnimationController &controller,
                      const json &animation_json);

void walk_2by3_preset(AnimationController &controller,
                      const json &animation_json);

} // namespace AnimationSerializer