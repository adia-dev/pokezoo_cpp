#pragma once

#include <config/config.h>

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

private:
  void init();
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

  // states
  bool _is_running = false;
  double _delta_time = 0.0;
  uint32_t _last_frame_ticks = 0;
};
