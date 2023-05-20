#include "application.h"

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
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    exit(EXIT_FAILURE);
  }

  // initialize SDL_ttf
  if (TTF_Init() != 0) {
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
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    exit(EXIT_FAILURE);
  }

  _renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(
      SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED),
      SDL_DestroyRenderer);

  if (_renderer == nullptr) {
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    exit(EXIT_FAILURE);
  }

  // set initial state
  _is_running = true;
  _last_frame_ticks = SDL_GetTicks();

  std::cout << "Application initialized" << std::endl;
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

void Application::render() {
  SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
  SDL_RenderClear(_renderer.get());
  SDL_RenderPresent(_renderer.get());
}

void Application::update() {
  uint32_t current_frame_ticks = SDL_GetTicks();
  _delta_time = (current_frame_ticks - _last_frame_ticks) / 1000.0;
  _last_frame_ticks = current_frame_ticks;
}

void Application::handle_events() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
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

  std::cout << "Application cleaned" << std::endl;
}