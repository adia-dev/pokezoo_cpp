#pragma once

#include <random>
#include <string>

class UUID {
public:
  UUID() = default;
  ~UUID() = default;

  // eheheh, this is not a real uuid, but it's good enough for now
  static std::string generate_v4_ish() {
    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::uniform_int_distribution<int> dist(0, 15);

    const char *v = "0123456789abcdef";
    const bool dash[] = {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0};

    std::string uuid;
    for (int i = 0; i < 16; i++) {
      if (dash[i])
        uuid += "-";
      uuid += v[dist(rng)];
      uuid += v[dist(rng)];
    }
    return uuid;
  }

private:
};