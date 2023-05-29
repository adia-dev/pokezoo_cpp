#include <animation/animation_controller.h>
#include <application/application.h>

#ifdef __EMSCRIPTEN__

extern "C" EMSCRIPTEN_KEEPALIVE int mainf() {
  try {
    Application::run();
  } catch (const std::exception &e) {
    LoggerManager::log_fatal(e.what());
    return EXIT_FAILURE;
  }

  return 0;
}

extern "C" EMSCRIPTEN_KEEPALIVE void quit() { Application::quit(); }

#else

int main() {
  try {
    Application::run();
  } catch (const std::exception &e) {
    LoggerManager::log_fatal(e.what());
    return EXIT_FAILURE;
  }

  return 0;
}

#endif