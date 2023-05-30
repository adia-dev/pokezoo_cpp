#include "application.h"
#include <animation/serializer.h>
#include <managers/input/input_manager.h>
#include <utils/render_utils.h>

void Application::run() {
  Application *app = Application::get();
  app->init();

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(
      [](void *arg) {
        auto app = static_cast<Application *>(arg);
        app->on_frame_start();
        app->handle_events();
        app->handle_input();
        app->update();
        app->render();
      },
      app, 0, 1);
#else
  while (app->_is_running) {
    app->on_frame_start();
    app->handle_events();
    app->handle_input();
    app->update();
    app->render();
  }

  app->clean();
#endif
}

void Application::quit() {
  auto app = get();
  app->_is_running = false;

#ifdef __EMSCRIPTEN__
  emscripten_cancel_main_loop();
#endif
}

void Application::init() {

  // initialize config
  // TODO: read from config file later
  _config = std::make_unique<ApplicationConfig>("My Game", DEFAULT_WINDOW_WIDTH,
                                                DEFAULT_WINDOW_HEIGHT,
                                                DEFAULT_WINDOW_FLAGS);

  // initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    LoggerManager::log_error("SDL_Init Error: " + std::string(SDL_GetError()));
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    exit(EXIT_FAILURE);
  }

  // initialize SDL_ttf
  if (TTF_Init() != 0) {
    LoggerManager::log_error("TTF_Init Error: " + std::string(TTF_GetError()));
    std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
    exit(EXIT_FAILURE);
  }

  // Init window and renderer
  SDL_CreateWindowAndRenderer(
      _config->window_config.width, _config->window_config.height,
      _config->window_config.flags, (SDL_Window **)&_window,
      (SDL_Renderer **)&_renderer);

  if (_window == nullptr) {
    LoggerManager::log_error("SDL_CreateWindow Error: " +
                             std::string(SDL_GetError()));
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    exit(EXIT_FAILURE);
  }

  if (_renderer == nullptr) {
    LoggerManager::log_error("SDL_CreateRenderer Error: " +
                             std::string(SDL_GetError()));
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    exit(EXIT_FAILURE);
  }

  SDL_SetRenderDrawBlendMode(_renderer.get(), SDL_BLENDMODE_BLEND);

  LoggerManager::init();

  // set initial state
  _is_running = true;
  _last_frame_ticks = SDL_GetTicks();

  adjust_window_scale();
  init_map();
  init_fonts();
  init_trainer();
  init_pokemons();

  LoggerManager::log_info("Application initialized");
}

void Application::init_map() {
  LoggerManager::log_info("Initializing map");
  _map = std::make_unique<Map>();
  _map->load("assets/maps/map1.json");
  LoggerManager::log_info("Initializing map done");
}

void Application::init_fonts() {
  LoggerManager::log_info("Initializing fonts");
  AssetManager::get_font("Roboto/Roboto-Regular.ttf", 16);
  LoggerManager::log_info("Initializing fonts done");
}

void Application::init_trainer() {
  LoggerManager::log_info("Initializing trainer");

  Trainer trainer("bw_overworld.png", 0, 0, 32, 32);
  trainer.set_name("Ash");

  _trainer = std::make_unique<Trainer>(trainer);
  AnimationSerializer::load_animations(_trainer->get_animation_controller(),
                                       "../src/assets/animations/"
                                       "pokemons/bw_overworld.json",
                                       "zorua");

  LoggerManager::log_info("Initializing trainer done");
}

void Application::init_pokemons() {
  LoggerManager::log_info("Initializing sprites");

  std::vector<std::string> pokemon_list = {
      "kyurem",    "pikachu",   "keldeo",    "boreas",   "fulguris",
      "demeteros", "cobaltium", "terrakium", "viridium", "victini",
      "munna",     "musharna",  "ratentif",  "zorua"};

  for (int i = 0; i < 1000; ++i) {
    int x = rand() % 1000;
    int y = rand() % 1000;
    int pokemon_index = rand() % pokemon_list.size();

    Pokemon new_pokemon("kyurem", "bw_overworld.png", x, y, 32, 32);
    new_pokemon.set_walk_speed(rand() % 75 + 25);
    if (pokemon_list[pokemon_index] == "kyurem") {
      new_pokemon.set_size(128, 128);
    }

    if (pokemon_list[pokemon_index] == "zorua") {
      new_pokemon.set_target(_trainer.get());
    }

    AnimationController animation_controller;
    AnimationSerializer::load_animations(
        animation_controller,
        "../src/assets/animations/pokemons/bw_overworld.json",
        pokemon_list[pokemon_index]);
    animation_controller.play_animation("idle_down");

    new_pokemon.attach_animation_controller(animation_controller);

    // _pokemons.push_back(std::make_unique<Pokemon>(new_pokemon));
  }

  auto loaded_pokemons =
      Pokemon::load_pokemons("../src/assets/json/bw_old_overworld.json");

  for (const auto &[name, pokemon] : loaded_pokemons) {
    _pokemons.push_back(std::make_unique<Pokemon>(pokemon));
  }

  LoggerManager::log_info("Initializing sprites done");
}

void Application::adjust_window_scale() {
  // Adjust the scale of the window because of HiDPI
  SDL_GetRendererOutputSize(_renderer.get(), &_config->window_config.width,
                            &_config->window_config.height);

  LoggerManager::log_warning(
      "Window size: " + std::to_string(_config->window_config.width) + "x" +
      std::to_string(_config->window_config.height));

  LoggerManager::log_warning(
      "Default window size: " + std::to_string(DEFAULT_WINDOW_WIDTH) + "x" +
      std::to_string(DEFAULT_WINDOW_HEIGHT));

  if (_config->window_config.width != DEFAULT_WINDOW_WIDTH ||
      _config->window_config.height != DEFAULT_WINDOW_HEIGHT) {
    float width_scale =
        (float)_config->window_config.width / DEFAULT_WINDOW_WIDTH;
    float height_scale =
        (float)_config->window_config.height / DEFAULT_WINDOW_HEIGHT;

    if (width_scale != height_scale) {
      LoggerManager::log_warning(
          "Window scale is not uniform. This may cause issues.");
    }

    SDL_RenderSetScale(_renderer.get(), width_scale, height_scale);
    _config->window_config.scale = {width_scale, height_scale};
    LoggerManager::log_warning("Window scale set to " +
                               std::to_string(width_scale) + "x" +
                               std::to_string(height_scale));
  }
}

void Application::update() {
  uint32_t current_frame_ticks = SDL_GetTicks();
  _fps = 1000 / std::max((current_frame_ticks - _last_frame_ticks), 10u);
  while (_fps > Application::get_config()->window_config.max_fps) {
    _fps = 1000 / std::max(SDL_GetTicks() - _last_frame_ticks, 10u);
  }
  _delta_time = (current_frame_ticks - _last_frame_ticks) / 1000.0f;
  _last_frame_ticks = current_frame_ticks;

  // update instances
  // _map->update(_delta_time);

  _delta_time = std::max(_delta_time, 0.016);

  if (_trainer) {
    _trainer->update(_delta_time);
  }

  // update sprites
  for (auto &sprite : _pokemons) {
    sprite->update(_delta_time);
  }
}

void Application::render() {
  SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
  SDL_RenderClear(_renderer.get());

  SDL_Texture *map_texture =
      AssetManager::get_texture("zoo.png", AssetDirectory::MAPS);
  // the map is the full size of the texture

  int map_width = _config->window_config.width / _config->window_config.scale.x;
  int map_height =
      _config->window_config.height / _config->window_config.scale.y;
  SDL_Rect map_rect = {
      0,
      0,
      map_width,
      map_height,
  };
  SDL_RenderCopy(_renderer.get(), map_texture, nullptr, &map_rect);

  RenderUtils::render_grid(_renderer.get(), _config->window_config.width,
                           _config->window_config.height,
                           _config->window_config.tile_size,
                           {255, 255, 255, 100});

  for (auto &sprite : _pokemons) {
    sprite->render(_renderer.get());
  }

  if (_trainer) {
    _trainer->render(_renderer.get());
  }

  Vector2i mouse_coords = Vector2i(InputManager::get_mouse_position()) /
                          _config->window_config.tile_size;

  RenderUtils::render_rect(_renderer.get(),
                           {mouse_coords.x * _config->window_config.tile_size,
                            mouse_coords.y * _config->window_config.tile_size,
                            _config->window_config.tile_size,
                            _config->window_config.tile_size},
                           {255, 0, 0, 100});

  std::stringstream ss;
  ss << "Mouse: " << mouse_coords << '\n';
  ss << "FPS: " << std::to_string(_fps) << '\n';
  ss << "Delta time: " << std::to_string(_delta_time) << '\n'
     << "Keyboard Direction: " << InputManager::get_directional_input() << '\n'
     << "Animation: "
     << _trainer->get_animation_controller().get_current_animation() << '\n';

  RenderUtils::render_text(
      _renderer.get(), AssetManager::get_font("Roboto/Roboto-Regular.ttf", 16),
      ss.str().c_str(), {255, 255, 255, 255}, 0, 0, true);

  SDL_RenderPresent(_renderer.get());
}

void Application::on_frame_start() { InputManager::update_key_states(); }

void Application::loop() {
  // same as run, for emscripten
  Application *app = Application::get();
  app->init();

  // lambda for emscripten
  auto loop = [](void *app_ptr) -> int {
    Application *app = static_cast<Application *>(app_ptr);
    app->on_frame_start();
    app->handle_events();
    app->update();
    app->render();
    return 0;
  };

  // emscripten_set_main_loop_arg(loop, this, 0, 1);
}

void Application::handle_events() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {

    InputManager::handle_event(event);

    switch (event.type) {
    case SDL_QUIT:
      _is_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        _is_running = false;
      }
      break;
      // handle window resize
    case SDL_WINDOWEVENT:
      if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
        adjust_window_scale();
      }
      break;
    default:
      break;
    }
  }
}

void Application::handle_input() {}

void Application::clean() {
  SDL_Quit();
  TTF_Quit();

  LoggerManager::get()->log_info("Cleaning up...");

  LoggerManager::get()->clean();
}
