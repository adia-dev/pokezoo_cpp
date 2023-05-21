#pragma once

#include <core/enums.h>
#include <string>

namespace EnumsUtils {

std::string direction_to_string(Direction direction);
std::string
animation_direction_to_string(AnimationDirection animation_direction);
std::string
supported_serializer_to_string(SupportedSerializer supported_serializer);
} // namespace EnumsUtils