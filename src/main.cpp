#include <application/application.h>

int main() {
  try {
    Application::run();
  } catch (const std::exception &e) {
    LoggerManager::log_fatal(e.what());
    return EXIT_FAILURE;
  }
  return 0;
}