#pragma once

#include <core/enums.h>
#include <string>
#include <unordered_map>

namespace EnumsUtils {

std::string direction_to_string(Direction direction);
Direction string_to_direction(const std::string &direction);
AnimationDirection string_to_animation_direction(const std::string &direction);
std::string
animation_direction_to_string(AnimationDirection animation_direction);
std::string
supported_serializer_to_string(SupportedSerializer supported_serializer);

Type type_from_string(const std::string &type);
std::string type_to_string(Type type);

static const std::unordered_map<Type, std::string> typeMap = {
    {Type::UNKNOWN, "UNKNOWN"}, {Type::NORMAL, "NORMAL"},
    {Type::FIRE, "FIRE"},       {Type::WATER, "WATER"},
    {Type::GRASS, "GRASS"},     {Type::ELECTRIC, "ELECTRIC"},
    {Type::ICE, "ICE"},         {Type::FIGHTING, "FIGHTING"},
    {Type::POISON, "POISON"},   {Type::GROUND, "GROUND"},
    {Type::FLYING, "FLYING"},   {Type::PSYCHIC, "PSYCHIC"},
    {Type::BUG, "BUG"},         {Type::ROCK, "ROCK"},
    {Type::GHOST, "GHOST"},     {Type::DARK, "DARK"},
    {Type::DRAGON, "DRAGON"},   {Type::STEEL, "STEEL"},
    {Type::FAIRY, "FAIRY"}};

} // namespace EnumsUtils
