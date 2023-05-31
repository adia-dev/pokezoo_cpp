#include "pokemon.h"
#include <animation/serializer.h>
#include <fstream>
#include <managers/asset/asset_manager.h>
#include <managers/logger/logger_manager.h>
#include <parsers/json.hpp>
#include <utils/enums_utils.h>

using json = nlohmann::json;

Pokemon::Pokemon(const std::string &name, const char *texture_name, int x,
                 int y, int width, int height, float scale)
    : Sprite(name, texture_name, x, y, width, height, scale) {}

Pokemon::Pokemon(const std::string &name, SDL_Texture *texture, int x, int y,
                 int width, int height, float scale)
    : Sprite(name, texture, x, y, width, height, scale) {}

void Pokemon::render(SDL_Renderer *renderer) {
  // Custom rendering logic for the Trainer class
  // Example: Render additional Trainer-specific elements

  // Call the base class render function to render the Sprite
  Sprite::render(renderer);
}

void Pokemon::update(double delta_time) {
  // Call the base class update function to update the Sprite
  Sprite::update(delta_time);

  if (_target != nullptr) {
    move_to(*_target, delta_time);
  }
}

void Pokemon::move_to(const Vector2i &target, double delta_time) {}

void Pokemon::move_to(const Sprite &target, double delta_time) {
  // get behind the target, same position, but 1 tile behind
  // get the direction from the target
  // move to the target

  // get behind the target, same position, but 1 tile behind
  Vector2i target_position = target.get_position();
  Vector2i target_direction = Vector2i::from_direction(target.get_direction());
  Vector2i behind_target_position = target_position - target_direction;

  // get the direction from the target
  Vector2i direction = (behind_target_position - get_position()).normalized();

  // move to the target
  move(direction.x * _walk_speed * delta_time,
       direction.y * _walk_speed * delta_time);

  // set the direction
  set_direction(Vector2i::direction_from_vector(direction));
}

std::map<std::string, Pokemon>
Pokemon::load_pokemons(const std::string &file_path) {

  std::ifstream file(file_path);
  if (!file.is_open()) {
    LoggerManager::log_fatal("Could not open file " + file_path);
    return {};
  }

  json json_data;
  try {
    file >> json_data;
  } catch (const std::exception &e) {
    LoggerManager::log_fatal("Could not parse JSON file " + file_path + ": " +
                             e.what());
    file.close();
    return {};
  }

  file.close();

  std::map<std::string, Pokemon> pokemons;

  if (!json_data.is_object()) {
    LoggerManager::log_fatal("JSON file " + file_path +
                             " does not contain a JSON object");
    return {};
  }

  if (!json_data.contains("texture_name") ||
      !json_data["texture_name"].is_string()) {
    LoggerManager::log_fatal("JSON file " + file_path +
                             " does not contain a texture_name string");
    return {};
  }

  if (!json_data.contains("pokemons") || !json_data["pokemons"].is_array()) {
    LoggerManager::log_fatal("JSON file " + file_path +
                             " does not contain a pokemons array");
    return {};
  }

  LoggerManager::log_debug(std::to_string(json_data["pokemons"].size()) +
                           " pokemons found in " + file_path);

  std::string texture_name = json_data["texture_name"].get<std::string>();

  SDL_Texture *texture = AssetManager::get_texture(texture_name.c_str());

  if (texture == nullptr) {
    LoggerManager::log_fatal("Texture " + texture_name + " does not exist");
    return {};
  }

  std::string pokemon_name;
  float walk_speed = 100.f;
  float run_speed = 200.f;
  float scale = 1.f;
  int width = 32;
  int height = 32;

  Stats stats;

  std::vector<Type> types;

  for (const auto &pokemon : json_data["pokemons"]) {

    try {
      pokemon_name = pokemon["name"].get<std::string>();

      pokemons[pokemon_name] =
          Pokemon(pokemon_name, texture, 0, 0, width, height, scale);

      if (pokemon.contains("properties") && pokemon["properties"].is_object()) {
        walk_speed = pokemon["properties"]["walk_speed"].get<float>();
        run_speed = pokemon["properties"]["run_speed"].get<float>();
        scale = pokemon["properties"]["scale"].get<float>();
        width = pokemon["properties"]["width"].get<int>();
        height = pokemon["properties"]["height"].get<int>();

        pokemons[pokemon_name].set_pokedex_id(
            std::to_string(pokemon["id"].get<int>()));
        pokemons[pokemon_name].set_name(pokemon_name);
        pokemons[pokemon_name].set_walk_speed(walk_speed);
        pokemons[pokemon_name].set_run_speed(run_speed);
        pokemons[pokemon_name].set_scale(scale);
        pokemons[pokemon_name].set_size(width, height);
      }

      if (pokemon.contains("stats") && pokemon["stats"].is_object()) {
        stats.hp = pokemon["stats"]["hp"].get<int>();
        stats.attack = pokemon["stats"]["attack"].get<int>();
        stats.defense = pokemon["stats"]["defense"].get<int>();
        stats.special_attack = pokemon["stats"]["spAttack"].get<int>();
        stats.special_defense = pokemon["stats"]["spDefense"].get<int>();
        stats.speed = pokemon["stats"]["speed"].get<int>();
        pokemons[pokemon_name].set_stats(stats);
      }

      if (pokemon.contains("types") && pokemon["types"].is_array()) {
        for (const auto &type : pokemon["types"]) {
          types.push_back(
              EnumsUtils::type_from_string(type.get<std::string>()));
        }
        pokemons[pokemon_name].set_types(types);
      }

      if (pokemon.contains("animations") && pokemon["animations"].is_array()) {
        const auto &animations_json = pokemon["animations"];
        auto &controller = pokemons[pokemon_name].get_animation_controller();
        for (const auto &animation_json : animations_json) {
          if (!animation_json.is_object()) {
            // Handle invalid animation JSON object error
            LoggerManager::log_warning("Could not parse JSON file " +
                                       file_path +
                                       ": invalid animation JSON object");
            continue;
          }

          if (animation_json.find("preset") != animation_json.end() &&
              animation_json["preset"].is_string()) {
            if (animation_json["preset"] == "walk") {
              AnimationSerializer::walk_preset(controller, animation_json);
            }

            if (animation_json["preset"] == "idle_down") {
              AnimationSerializer::idle_down_preset(controller, animation_json);
            }

            if (animation_json["preset"] == "walk_2x3_flipped") {
              AnimationSerializer::walk_2by3_preset(controller, animation_json);
            }
            continue;
          }

          Animation new_animation;
          AnimationSerializer::process_animation_json(animation_json,
                                                      new_animation);
          controller.add_animation(new_animation.name, new_animation);
        }
      }

    } catch (const std::exception &e) {
      LoggerManager::log_warning("Could not parse pokemon " + pokemon_name +
                                 ": " + e.what());
    }
  }

  LoggerManager::log_debug("Loaded " + std::to_string(pokemons.size()) +
                           " pokemons from " + file_path);

  // names
  LoggerManager::log_debug("Pokemons: " + [&]() {
    std::string names;
    for (const auto &pokemon : pokemons) {
      names += pokemon.first + ", ";
    }
    return names;
  }());

  return pokemons;
}
