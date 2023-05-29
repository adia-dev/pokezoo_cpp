#include "enums_utils.h"

std::string EnumsUtils::direction_to_string(Direction direction) {
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

std::string
animation_direction_to_string(AnimationDirection animation_direction) {
  switch (animation_direction) {
  case AnimationDirection::FORWARD:
    return "FORWARD";
  case AnimationDirection::REVERSE:
    return "REVERSE";
  case AnimationDirection::LOOP:
    return "LOOP";
  case AnimationDirection::PING_PONG:
    return "PING PONG";
  default:
    return "UNKNOWN";
  }
}

std::string
supported_serializer_to_string(SupportedSerializer supported_serializer) {
  switch (supported_serializer) {
  case SupportedSerializer::JSON:
    return "JSON";
  case SupportedSerializer::XML:
    return "XML";
  default:
    return "UNKNOWN";
  }
}