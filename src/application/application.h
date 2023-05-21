#pragma once

#include <core/config.h>
#include <managers/asset/asset_manager.h>
#include <managers/logger/logger_manager.h>
#include <map/map.h>
#include <sprite/sprite.h>

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

  bool is_running() const { return _is_running; }

  double get_delta_time() const { return _delta_time; }
  uint32_t get_last_frame_ticks() const { return _last_frame_ticks; }

  SDL_Renderer *get_renderer() { return _renderer.get(); }
  SDL_Window *get_window() { return _window.get(); }

  static ApplicationConfig *get_config() { return get()->_config.get(); }

  void set_config(std::unique_ptr<ApplicationConfig> config) {
    _config = std::move(config);
  }

  void set_map(std::unique_ptr<Map> map) { _map = std::move(map); }

private:
  void init();
  void init_sprites();

  void loop();
  void render();
  void update();
  void handle_events();
  void clean();

  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> _window = {
      nullptr, SDL_DestroyWindow};
  std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> _renderer = {
      nullptr, SDL_DestroyRenderer};

  // config
  std::unique_ptr<ApplicationConfig> _config = nullptr;

  // instances
  std::unique_ptr<Map> _map = nullptr;
  std::vector<std::unique_ptr<Sprite>> _sprites;

  // states
  bool _is_running = false;
  double _delta_time = 0.0;
  uint32_t _last_frame_ticks = 0;
};
