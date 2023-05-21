#include "string_utils.h"

std::string StringUtils::to_lower(const std::string &str) {
  std::string lower_str = str;

  for (char &c : lower_str) {
    c = tolower(c);
  }

  return lower_str;
}

std::string StringUtils::to_upper(const std::string &str) {
  std::string upper_str = str;

  for (char &c : upper_str) {
    c = toupper(c);
  }

  return upper_str;
}
