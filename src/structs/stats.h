#pragma once

#include <iostream>
#include <ostream>

struct Stats {
  int hp;
  int attack;
  int defense;
  int special_attack;
  int special_defense;
  int speed;

  friend std::ostream &operator<<(std::ostream &os, const Stats &stats) {
    os << "hp: " << stats.hp << "\n";
    os << "attack: " << stats.attack << "\n";
    os << "defense: " << stats.defense << "\n";
    os << "special_attack: " << stats.special_attack << "\n";
    os << "special_defense: " << stats.special_defense << "\n";
    os << "speed: " << stats.speed << "\n";
    return os;
  }
};
