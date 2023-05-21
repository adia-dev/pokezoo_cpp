#include <application/application.h>
#include <fstream>
#include <http/request.h>
#include <parsers/json.hpp>

int main() {
  // try {
  //   Application::run();
  // } catch (const std::exception &e) {
  //   LoggerManager::log_fatal(e.what());
  //   return EXIT_FAILURE;
  // }

  std::ifstream f("cache/tmp.json");
  if (!f.is_open()) {
    std::cout << "Failed to open file" << std::endl;
    return 1;
  }
  nlohmann::json j;
  f >> j;
  f.close();

  std::cout << j["abilities"] << std::endl;

  return 0;
}