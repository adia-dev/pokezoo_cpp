#include "application.h"
#include <animation/serializer.h>
#include <managers/input/input_manager.h>
#include <utils/render_utils.h>

void Application::run() {
  Application *app = Application::get();
  app->init();

  while (app->_is_running) {
    app->handle_events();
    app->update();
    app->render();
  }

  app->clean();
}

void Application::init() {

  // initialize config
  // TODO: read from config file later
  _config = std::make_unique<ApplicationConfig>("My Game", DEFAULT_WINDOW_WIDTH,
                                                DEFAULT_WINDOW_HEIGHT,
                                                DEFAULT_WINDOW_FLAGS);

  // initialize SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
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
  _window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(
      SDL_CreateWindow(_config->window_config.title, SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, _config->window_config.width,
                       _config->window_config.height,
                       _config->window_config.flags),
      SDL_DestroyWindow);

  if (_window == nullptr) {
    LoggerManager::log_error("SDL_CreateWindow Error: " +
                             std::string(SDL_GetError()));
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    exit(EXIT_FAILURE);
  }

  _renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(
      SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED),
      SDL_DestroyRenderer);

  if (_renderer == nullptr) {
    LoggerManager::log_error("SDL_CreateRenderer Error: " +
                             std::string(SDL_GetError()));
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    exit(EXIT_FAILURE);
  }

  SDL_SetRenderDrawBlendMode(_renderer.get(), SDL_BLENDMODE_BLEND);

  LoggerManager::init();

  // init instances
  _map = std::make_unique<Map>();
  _map->load("assets/maps/map1.json");

  // set initial state
  _is_running = true;
  _last_frame_ticks = SDL_GetTicks();

  init_trainer();
  init_sprites();

  LoggerManager::log_info("Application initialized");
}

void Application::init_trainer() {
  LoggerManager::log_info("Initializing trainer");

  Trainer trainer("bw_characters.png", 0, 0, 32, 32);
  trainer.set_name("Ash");

  _trainer = std::make_unique<Trainer>(trainer);
  AnimationSerializer::load_animations(
      _trainer->get_animation_controller(),
      "/Users/abdoulayedia/Projects/Dev/C++/pokezoo/src/assets/animations/"
      "trainers/bw_male.json");

  LoggerManager::log_info("Initializing trainer done");
}

void Application::init_sprites() {
  LoggerManager::log_info("Initializing sprites");

  Sprite new_sprite("pokemons_4th_gen.png", 0, 0, 32, 32);
  AnimationController animation_controller;
  Animation idle_up("idle_up");
  idle_up.add_frame(Frame({0, 0, 32, 32}, 100));
  idle_up.add_frame(Frame({32, 0, 32, 32}, 100));
  idle_up.add_frame(Frame({64, 0, 32, 32}, 100));
  idle_up.add_frame(Frame({96, 0, 32, 32}, 100));

  animation_controller.add_animation("idle_up", idle_up);
  animation_controller.play_animation("idle_up");

  new_sprite.attach_animation_controller(animation_controller);

  _sprites.push_back(std::make_unique<Sprite>(new_sprite));

  LoggerManager::log_info("Initializing sprites done");
}

void Application::update() {
  uint32_t current_frame_ticks = SDL_GetTicks();
  int fps = 1000 / (current_frame_ticks - _last_frame_ticks);
  while (fps > Application::get_config()->window_config.max_fps) {
    fps = 1000 / (SDL_GetTicks() - _last_frame_ticks);
  }
  _delta_time = (current_frame_ticks - _last_frame_ticks) / 1000.0f;
  _last_frame_ticks = current_frame_ticks;

  // update instances
  // _map->update(_delta_time);

  if (_trainer) {
    _trainer->update(_delta_time);
  }

  // update sprites
  for (auto &sprite : _sprites) {
    sprite->update(_delta_time);
  }
}

void Application::render() {
  SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
  SDL_RenderClear(_renderer.get());

  RenderUtils::render_grid(_renderer.get(), _config->window_config.width,
                           _config->window_config.height,
                           _config->window_config.tile_size,
                           {255, 255, 255, 100});

  for (auto &sprite : _sprites) {
    sprite->render(_renderer.get());
  }

  if (_trainer) {
    _trainer->render(_renderer.get());
  }

  Vector2i mouse_coords = InputManager::get_mouse_position() / 32;
  RenderUtils::render_rect(_renderer.get(),
                           {mouse_coords.x * 32, mouse_coords.y * 32, 32, 32},
                           {255, 0, 0, 100});

  SDL_RenderPresent(_renderer.get());
}

void Application::loop() {
  // same as run, for emscripten
  Application *app = Application::get();
  app->init();

  // lambda for emscripten
  auto loop = [](void *app_ptr) -> int {
    Application *app = static_cast<Application *>(app_ptr);
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

    InputManager::get()->handle_events(event);

    switch (event.type) {
    case SDL_QUIT:
      _is_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        _is_running = false;
      }
      break;
    default:
      break;
    }
  }
}

void Application::clean() {
  SDL_Quit();
  TTF_Quit();

  LoggerManager::get()->log_info("Cleaning up...");

  LoggerManager::get()->clean();
}