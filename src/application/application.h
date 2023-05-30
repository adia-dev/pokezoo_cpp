#pragma once

#include <core/config.h>
#include <managers/asset/asset_manager.h>
#include <managers/logger/logger_manager.h>
#include <map/map.h>
#include <sprite/pokemon.h>
#include <sprite/sprite.h>
#include <sprite/trainer.h>

class Application {
public:
  Application() = default;
  ~Application() = default;

  static Application *get() {
    static std::unique_ptr<Application> instance =
        std::make_unique<Application>();
    return instance.get();
  }

  static void run();
  static void quit();

  bool is_running() const { return _is_running; }

  double get_delta_time() const { return _delta_time; }
  uint32_t get_last_frame_ticks() const { return _last_frame_ticks; }

  SDL_Renderer *get_renderer() { return _renderer.get(); }
  SDL_Window *get_window() { return _window.get(); }

  static ApplicationConfig *get_config() { return get()->_config.get(); }

  static void set_selected_pokemon(std::shared_ptr<Pokemon> pokemon) {
    get()->_selected_pokemon = pokemon;
  }

  static std::shared_ptr<Pokemon> get_selected_pokemon() {
    return get()->_selected_pokemon;
  }

  void set_config(std::unique_ptr<ApplicationConfig> config) {
    _config = std::move(config);
  }

  void set_map(std::unique_ptr<Map> map) { _map = std::move(map); }

private:
  void init();
  void init_map();
  void init_fonts();
  void init_trainer();
  void init_pokemons();

  void adjust_window_scale();

  void on_frame_start();
  void loop();
  void render();
  void update();
  void handle_events();
  void handle_input();
  void clean();

  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window = {
      nullptr, SDL_DestroyWindow};
  std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer = {
      nullptr, SDL_DestroyRenderer};

  // config
  std::unique_ptr<ApplicationConfig> _config = nullptr;

  // instances
  std::unique_ptr<Map> _map = nullptr;
  std::vector<std::shared_ptr<Pokemon>> _pokemons;
  std::unique_ptr<Trainer> _trainer = nullptr;
  std::shared_ptr<Pokemon> _selected_pokemon = nullptr;

  // states
  bool _is_running = false;
  double _delta_time = 0.0;
  int _fps = 0;
  uint32_t _last_frame_ticks = 0;
};
