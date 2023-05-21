#pragma once

#include <core/config.h>
#include <curl/curl.h>
#include <managers/logger/logger_manager.h>

struct RequestResponse {
  char *body;
  size_t status_code;
};

enum class RequestType { GET, POST, PUT, PATCH, DELETE };

class Request {
public:
  Request() = default;
  ~Request() = default;

  // I usually name this method `get` but it conflicts with the `get` method
  static Request *instance() {
    static std::unique_ptr<Request> instance = std::make_unique<Request>();
    return instance.get();
  }

  static void init();
  static void clean();

  // methods
  static RequestResponse
  request(RequestType type, const std::string &url,
          const std::vector<std::pair<std::string, std::string>> &headers = {},
          const std::vector<std::pair<std::string, std::string>> &params = {},
          const std::string &body = "");

  static RequestResponse
  get(const std::string &url,
      const std::vector<std::pair<std::string, std::string>> &headers = {},
      const std::vector<std::pair<std::string, std::string>> &params = {});

  static RequestResponse
  post(const std::string &url,
       const std::vector<std::pair<std::string, std::string>> &headers = {},
       const std::vector<std::pair<std::string, std::string>> &params = {},
       const std::string &body = "");

  static RequestResponse
  put(const std::string &url,
      const std::vector<std::pair<std::string, std::string>> &headers = {},
      const std::vector<std::pair<std::string, std::string>> &params = {},
      const std::string &body = "");

  static RequestResponse
  patch(const std::string &url,
        const std::vector<std::pair<std::string, std::string>> &headers = {},
        const std::vector<std::pair<std::string, std::string>> &params = {},
        const std::string &body = "");

  static std::string request_type_to_string(RequestType type);

private:
  CURL *_curl = nullptr;
  std::unordered_map<std::string, std::stack<RequestResponse>> _cache;

  static size_t write_callback(void *contents, size_t size, size_t n_memb,
                               void *user_p);

  static size_t read_callback(void *ptr, size_t size, size_t n_memb,
                              void *user_p);

  static std::string
  format_params(const std::vector<std::pair<std::string, std::string>> &params);

  static std::string format_headers(
      const std::vector<std::pair<std::string, std::string>> &headers);
};