
#pragma once

#include "sprite.h"
#include <filesystem>

class Pokemon : public Sprite {
public:
  Pokemon() = default;
  Pokemon(const std::string &name, const char *texture_name, int x, int y,
          int width, int height, float scale = 1.f);
  Pokemon(const std::string &name, SDL_Texture *texture, int x, int y,
          int width, int height, float scale = 1.f);
  ~Pokemon() override = default;

  void render(SDL_Renderer *renderer) override;
  void update(double delta_time) override;

  void move_to(const Vector2i &target, double delta_time);
  void move_to(const Sprite &target, double delta_time);

  void set_name(const std::string &name) { _name = name; }
  const std::string &get_name() const { return _name; }

  void set_target(Sprite *target) { _target = target; }
  void release_target() { _target = nullptr; }

  void set_walk_speed(float speed) { _walk_speed = speed; }

  void set_run_speed(float speed) { _run_speed = speed; }

  static std::map<std::string, Pokemon>
  load_pokemons(const std::string &file_path);

  friend std::ostream &operator<<(std::ostream &os, const Pokemon &pokemon) {
    // print it like a json object
    os << "{\n";
    os << "\t\"type\": \"pokemon\",\n";
    os << "\t\"id\": " << pokemon.get_id() << ",\n";
    os << "\t\"texture\": \"" << pokemon.get_texture() << "\",\n";
    os << "\t\"name\": \"" << pokemon._name << "\",\n";
    os << "\t\"walk_speed\": " << pokemon._walk_speed << ",\n";
    os << "\t\"run_speed\": " << pokemon._run_speed << ",\n";
    os << "\t\"speed\": " << pokemon._speed << ",\n";
    os << "\t\"desired_speed\": " << pokemon._desired_speed << ",\n";
    os << "\t\"is_running\": " << pokemon._is_running << ",\n";
    os << "\t\"target\": " << pokemon._target << "\n";
    os << "}";
    return os;
  }

private:
  std::string _name;
  float _walk_speed = 200;
  float _run_speed = 300;
  float _speed = _walk_speed;
  float _desired_speed = _walk_speed;
  bool _is_running = false;

  Sprite *_target = nullptr;
};
