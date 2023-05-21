#include <application/application.h>
#include <http/request.h>

int main() {
  // try {
  //   Application::run();
  // } catch (const std::exception &e) {
  //   LoggerManager::log_fatal(e.what());
  //   return EXIT_FAILURE;
  // }

  /*

    pub struct NewUser {
    pub firebase_id: String,
    pub email: String,
    pub gender: String,
    pub username: Option<String>,
    pub bio: Option<String>,
}

  write a string json of a new user
  */

  const char *new_user_json = R"(
  {
    "firebase_id": "123456789",
    "email": "abdoudu78130@gmail.com",
    "gender": "MALE",
    "username": "abdoudu78130",
    "bio": "I am a software engineer"
  }
  )";

  Request::instance()->init();
  try {
    auto response = Request::get("http://localhost:5172/users/list",
                                 {{"Content-Type", "application/json"},
                                  {"User-Agent", "curl/7.64.1"},
                                  {"Accept", "*/*"}},
                                 {});
  } catch (const std::exception &e) {
    LoggerManager::log_fatal(e.what());
    return EXIT_FAILURE;
  }

  return 0;
}