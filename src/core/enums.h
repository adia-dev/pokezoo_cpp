#pragma once

// CORE
enum class Direction { NONE, UP, DOWN, LEFT, RIGHT, COUNT };

// ANIMATION
enum class AnimationDirection { FORWARD, REVERSE, LOOP, ONCE, PINGPONG, COUNT };

// SPRITE
enum class SupportedSerializer { JSON, XML, COUNT };

// MAP
enum LayerType { TILE_LAYER, OBJECT_LAYER, OBSTACLE_LAYER };
enum class TileType { NONE, FLOOR, WALL, COUNT };
