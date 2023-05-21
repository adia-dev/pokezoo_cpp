#include "request.h"

void Request::init() {
  auto *request = Request::instance();
  request->_curl = curl_easy_init();
  if (!request->_curl) {
    std::cerr << "Failed to initialize CURL" << std::endl;
    exit(EXIT_FAILURE);
  }

  curl_easy_setopt(request->_curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(request->_curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
  curl_easy_setopt(request->_curl, CURLOPT_TIMEOUT, 5L);
  curl_easy_setopt(request->_curl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);
  curl_easy_setopt(request->_curl, CURLOPT_XOAUTH2_BEARER, "token");
  curl_easy_setopt(request->_curl, CURLOPT_WRITEFUNCTION,
                   Request::write_callback);
  curl_easy_setopt(request->_curl, CURLOPT_READFUNCTION,
                   Request::read_callback);
}

void Request::clean() {
  LoggerManager::log_info("Cleaning request...");
  auto *request = Request::instance();
  if (request == nullptr) {
    return;
  }

  if (request && request->_curl != nullptr) {
    curl_easy_cleanup(request->_curl);
  }

  curl_global_cleanup();
  request->_cache.clear();

  LoggerManager::log_info("Request cleaned");
}

RequestResponse Request::request(
    RequestType type, const std::string &url,
    const std::vector<std::pair<std::string, std::string>> &headers,
    const std::vector<std::pair<std::string, std::string>> &params,
    const std::string &body) {
  auto *request = Request::instance();
  RequestResponse response;
  response.body = nullptr;
  response.status_code = 0;

  // set headers
  struct curl_slist *chunk = nullptr;
  for (const auto &[key, value] : headers) {
    chunk = curl_slist_append(chunk, (key + ": " + value).c_str());
  }
  curl_easy_setopt(request->_curl, CURLOPT_HTTPHEADER, chunk);

  std::string url_with_params = url + Request::format_params(params);

  // set the URL
  curl_easy_setopt(request->_curl, CURLOPT_URL, url_with_params.c_str());

  // set response
  curl_easy_setopt(request->_curl, CURLOPT_WRITEDATA, &response);

  // set body if any
  if (!body.empty()) {
    curl_easy_setopt(request->_curl, CURLOPT_POSTFIELDS, body.c_str());
  }

  // set the method, if unknown, default to GET
  std::string method_name = request_type_to_string(type);
  if (method_name == "UNKOWN") {
    method_name = "GET";
  }

  curl_easy_setopt(request->_curl, CURLOPT_CUSTOMREQUEST, method_name.c_str());

  // perform the request
  CURLcode res = curl_easy_perform(request->_curl);
  if (res != CURLE_OK) {
    LoggerManager::log_error("curl_easy_perform() failed: " +
                             std::string(curl_easy_strerror(res)));
    return response;
  }

  // get response code
  long response_code;
  curl_easy_getinfo(request->_curl, CURLINFO_RESPONSE_CODE, &response_code);
  response.status_code = static_cast<size_t>(response_code);

  // cleanup
  curl_slist_free_all(chunk);

  LoggerManager::log_info("Sent a " +
                          std::string(request_type_to_string(type)) +
                          " request to " + url);
  // params, headers,and body
  LoggerManager::log_info("Request params: " + Request::format_params(params));
  LoggerManager::log_info("Request headers: \n" +
                          Request::format_headers(headers));
  LoggerManager::log_info("Request body: " + body);
  LoggerManager::log_info("Response status: " +
                          std::to_string(response.status_code));

  if (response.body != nullptr)
    LoggerManager::log_info("Response body: " + std::string(response.body));
  else
    LoggerManager::log_info("Response body: " + std::string("NULL"));

  return response;
}

RequestResponse
Request::get(const std::string &url,
             const std::vector<std::pair<std::string, std::string>> &headers,
             const std::vector<std::pair<std::string, std::string>> &params) {
  return Request::request(RequestType::GET, url, headers, params);
}

RequestResponse
Request::post(const std::string &url,
              const std::vector<std::pair<std::string, std::string>> &headers,
              const std::vector<std::pair<std::string, std::string>> &params,
              const std::string &body) {
  return Request::request(RequestType::POST, url, headers, params, body);
}

RequestResponse
Request::put(const std::string &url,
             const std::vector<std::pair<std::string, std::string>> &headers,
             const std::vector<std::pair<std::string, std::string>> &params,
             const std::string &body) {
  return Request::request(RequestType::PUT, url, headers, params, body);
}

RequestResponse
Request::patch(const std::string &url,
               const std::vector<std::pair<std::string, std::string>> &headers,
               const std::vector<std::pair<std::string, std::string>> &params,
               const std::string &body) {
  return Request::request(RequestType::PATCH, url, headers, params, body);
}

std::string Request::request_type_to_string(RequestType type) {
  switch (type) {
  case RequestType::GET:
    return "GET";
  case RequestType::POST:
    return "POST";
  case RequestType::PUT:
    return "PUT";
  case RequestType::DELETE:
    return "DELETE";
  case RequestType::PATCH:
    return "PATCH";
  default:
    return "UNKNOWN";
  }
}

size_t Request::write_callback(void *contents, size_t size, size_t n_memb,
                               void *user_p) {
  if (user_p == nullptr) {
    LoggerManager::log_error("Could not write to user_p");
    return 0;
  }

  if (contents == nullptr) {
    LoggerManager::log_error("Could not write to contents");
    return 0;
  }

  size_t real_size = size * n_memb;
  RequestResponse *response = static_cast<RequestResponse *>(user_p);

  response->body = static_cast<char *>(
      realloc(response->body, response->status_code + real_size + 1));
  if (response->body == nullptr) {
    LoggerManager::log_error("Could not realloc response body");
    return 0;
  }

  memcpy(&(response->body[response->status_code]), contents, real_size);
  response->status_code += real_size;
  response->body[response->status_code] = '\0';

  return real_size;
}

size_t Request::read_callback(void *ptr, size_t size, size_t n_memb,
                              void *user_p) {

  LoggerManager::log_info("read_callback");

  if (user_p == nullptr) {
    LoggerManager::log_error("Could not read from user_p");
    return 0;
  }

  if (ptr == nullptr) {
    LoggerManager::log_error("Could not read from ptr");
    return 0;
  }

  size_t real_size = size * n_memb;
  RequestResponse *response = static_cast<RequestResponse *>(user_p);

  if (real_size < 1) {
    return 0;
  }

  if (response->status_code == 0) {
    return 0;
  }

  if (response->status_code < real_size) {
    real_size = response->status_code;
  }

  memcpy(ptr, response->body, real_size);
  response->body += real_size;
  response->status_code -= real_size;

  return real_size;
}

std::string Request::format_params(
    const std::vector<std::pair<std::string, std::string>> &params) {
  if (params.size() == 0) {
    return "";
  }

  std::string params_str;
  // format the params to be like: param1=value1&param2=value2, add a ? at the
  // beginning

  for (size_t i = 0; i < params.size(); i++) {
    const auto &[key, value] = params[i];
    params_str += key + "=" + value;
    if (i != params.size() - 1) {
      params_str += "&";
    }
  }

  return "?" + params_str;
}

std::string Request::format_headers(
    const std::vector<std::pair<std::string, std::string>> &headers) {
  if (headers.size() == 0) {
    return "";
  }

  std::string headers_str;
  // format the headers to be like: header1: value1\nheader2: value2\n

  for (size_t i = 0; i < headers.size(); i++) {
    const auto &[key, value] = headers[i];
    headers_str += key + ": " + value;
    if (i != headers.size() - 1) {
      headers_str += "\n";
    }
  }

  return headers_str;
}
