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
    return "NONE";
  }
}

Direction EnumsUtils::string_to_direction(const std::string &direction) {
  if (direction == "UP")
    return Direction::UP;
  if (direction == "DOWN")
    return Direction::DOWN;
  if (direction == "LEFT")
    return Direction::LEFT;
  if (direction == "RIGHT")
    return Direction::RIGHT;
  return Direction::NONE;
}

AnimationDirection
EnumsUtils::string_to_animation_direction(const std::string &direction) {
  if (direction == "FORWARD")
    return AnimationDirection::FORWARD;
  if (direction == "REVERSE")
    return AnimationDirection::REVERSE;
  if (direction == "LOOP")
    return AnimationDirection::LOOP;
  if (direction == "PING PONG")
    return AnimationDirection::PING_PONG;
  return AnimationDirection::LOOP;
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

std::string EnumsUtils::supported_serializer_to_string(
    SupportedSerializer supported_serializer) {
  switch (supported_serializer) {
  case SupportedSerializer::JSON:
    return "JSON";
  case SupportedSerializer::XML:
    return "XML";
  default:
    return "UNKNOWN";
  }
}

Type EnumsUtils::type_from_string(const std::string &type) {
  if (type == "NORMAL")
    return Type::NORMAL;
  if (type == "FIRE")
    return Type::FIRE;
  if (type == "WATER")
    return Type::WATER;
  if (type == "GRASS")
    return Type::GRASS;
  if (type == "ELECTRIC")
    return Type::ELECTRIC;
  if (type == "ICE")
    return Type::ICE;
  if (type == "FIGHTING")
    return Type::FIGHTING;
  if (type == "POISON")
    return Type::POISON;
  if (type == "GROUND")
    return Type::GROUND;
  if (type == "FLYING")
    return Type::FLYING;
  if (type == "PSYCHIC")
    return Type::PSYCHIC;
  if (type == "BUG")
    return Type::BUG;
  if (type == "ROCK")
    return Type::ROCK;
  if (type == "GHOST")
    return Type::GHOST;
  if (type == "DARK")
    return Type::DARK;
  if (type == "DRAGON")
    return Type::DRAGON;
  if (type == "STEEL")
    return Type::STEEL;
  if (type == "FAIRY")
    return Type::FAIRY;
  return Type::UNKNOWN;
}

std::string type_to_string(Type type) {
  switch (type) {
  case Type::NORMAL:
    return "NORMAL";
  case Type::FIRE:
    return "FIRE";
  case Type::WATER:
    return "WATER";
  case Type::GRASS:
    return "GRASS";
  case Type::ELECTRIC:
    return "ELECTRIC";
  case Type::ICE:
    return "ICE";
  case Type::FIGHTING:
    return "FIGHTING";
  case Type::POISON:
    return "POISON";
  case Type::GROUND:
    return "GROUND";
  case Type::FLYING:
    return "FLYING";
  case Type::PSYCHIC:
    return "PSYCHIC";
  case Type::BUG:
    return "BUG";
  case Type::ROCK:
    return "ROCK";
  case Type::GHOST:
    return "GHOST";
  case Type::DARK:
    return "DARK";
  case Type::DRAGON:
    return "DRAGON";
  case Type::STEEL:
    return "STEEL";
  case Type::FAIRY:
    return "FAIRY";
  default:
    return "COUNT";
  }
}
