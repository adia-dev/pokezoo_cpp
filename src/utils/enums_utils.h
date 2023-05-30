#pragma once

#include <core/enums.h>
#include <string>

namespace EnumsUtils {

std::string direction_to_string(Direction direction);
std::string
animation_direction_to_string(AnimationDirection animation_direction);
std::string
supported_serializer_to_string(SupportedSerializer supported_serializer);

Type type_from_string(const std::string &type);
std::string type_to_string(Type type);

} // namespace EnumsUtils
