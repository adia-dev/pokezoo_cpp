#pragma once

#include <core/config.h>
#include <core/enums.h>
#include <managers/logger/logger_manager.h>
#include <structs/my_vector.h>

struct Tile {

  int id;
  Vector2i coords;
  TileType type;
  int heuristic;
  int cost;

  Tile()
      : id(0), coords(Vector2i()), type(TileType::NONE), heuristic(0), cost(0) {
  }

  void set_id(int id) { this->id = id; }
  void set_coords(Vector2i coords) { this->coords = coords; }
  void set_type(TileType type) { this->type = type; }
  void set_heuristic(int heuristic) { this->heuristic = heuristic; }
  void set_cost(int cost) { this->cost = cost; }

  int get_id() const { return id; }
  const Vector2i &get_coords() const { return coords; }
  TileType get_type() const { return type; }
  int get_heuristic() const { return heuristic; }
  int get_cost() const { return cost; }
};