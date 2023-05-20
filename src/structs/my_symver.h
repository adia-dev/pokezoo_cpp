#pragma once

#include <config/config.h>

// symver
struct Version {
  int major;
  int minor;
  int patch;

  bool operator==(const Version &other) const {
    return major == other.major && minor == other.minor && patch == other.patch;
  }

  bool operator!=(const Version &other) const { return !(*this == other); }

  bool operator<(const Version &other) const {
    if (major < other.major) {
      return true;
    } else if (major > other.major) {
      return false;
    } else {
      if (minor < other.minor) {
        return true;
      } else if (minor > other.minor) {
        return false;
      } else {
        return patch < other.patch;
      }
    }
  }

  bool operator>(const Version &other) const {
    return !(*this < other) && *this != other;
  }

  bool operator<=(const Version &other) const {
    return *this < other || *this == other;
  }

  bool operator>=(const Version &other) const {
    return *this > other || *this == other;
  }

  friend std::ostream &operator<<(std::ostream &os, const Version &version) {
    os << version.major << "." << version.minor << "." << version.patch;
    return os;
  }

  std::string to_string() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }
};