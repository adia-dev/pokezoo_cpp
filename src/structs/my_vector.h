#pragma once

#include <cmath>
#include <core/enums.h>
#include <iostream>

template <typename T> class Vector2 {
public:
  Vector2() = default;
  Vector2(T x, T y) : x(x), y(y) {}
  Vector2(const Vector2 &other) = default;
  ~Vector2() = default;

  template <typename U> Vector2(const Vector2<U> &other) {
    x = other.x;
    y = other.y;
  }

  template <typename U> Vector2 &operator=(const Vector2<U> &other) {
    x = other.x;
    y = other.y;
    return *this;
  }

  T x = 0;
  T y = 0;

  // Magnitude of the vector
  T magnitude() const { return std::sqrt(x * x + y * y); }

  // lerp between two vectors
  Vector2 lerp(const Vector2 &other, float t) const {
    return Vector2(x + (other.x - x) * t, y + (other.y - y) * t);
  }

  // Distance between two vectors
  T distance(const Vector2 &other) const {
    return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
  }

  // Manhattan distance between two vectors
  T manhattan_distance(const Vector2 &other) const {
    return std::abs(x - other.x) + std::abs(y - other.y);
  }

  // Dot product of two vectors
  T dot_product(const Vector2 &other) const {
    return x * other.x + y * other.y;
  }

  // Cross product of two vectors
  T cross_product(const Vector2 &other) const {
    return x * other.y - y * other.x;
  }

  Vector2 &to_coords(int tile_size) {
    x /= tile_size;
    y /= tile_size;
    return *this;
  }

  Vector2 &to_pixels(int tile_size) {
    x *= tile_size;
    y *= tile_size;
    return *this;
  }

  Vector2 &normalized() {
    T mag = magnitude();

    if (mag == 0) {
      return *this;
    }

    x /= mag;
    y /= mag;
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &os, const Vector2 &v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
  }

  // Static versions of the functions

  static T magnitude(const Vector2 &v) { return v.magnitude(); }

  static T manhattan_distance(const Vector2 &v1, const Vector2 &v2) {
    return v1.manhattan_distance(v2);
  }

  static T dot_product(const Vector2 &v1, const Vector2 &v2) {
    return v1.dot_product(v2);
  }

  static T cross_product(const Vector2 &v1, const Vector2 &v2) {
    return v1.cross_product(v2);
  }

  static Vector2 lerp(const Vector2 &v1, const Vector2 &v2, float t) {
    return v1.lerp(v2, t);
  }

  static Vector2 to_coords(const Vector2 &v, int tile_size) {
    return Vector2(v.x / tile_size, v.y / tile_size);
  }

  static Vector2 to_pixels(const Vector2 &v, int tile_size) {
    return Vector2(v.x * tile_size, v.y * tile_size);
  }

  static Vector2<T> zero() { return Vector2<T>(0, 0); }
  static Vector2<T> one() { return Vector2<T>(1, 1); }

  static Vector2<T> up() { return Vector2<T>(0, -1); }
  static Vector2<T> down() { return Vector2<T>(0, 1); }
  static Vector2<T> left() { return Vector2<T>(-1, 0); }
  static Vector2<T> right() { return Vector2<T>(1, 0); }

  static Vector2<T> unit() { return Vector2<T>(1, 1); }

  static Direction direction_from_vector(const Vector2<T> &v) {
    float x = v.x;
    float y = v.y;

    if (x == 0 && y == 0) {
      return Direction::NONE;
    }

    if (std::abs(x) > std::abs(y)) {
      if (x > 0) {
        return Direction::RIGHT;
      } else {
        return Direction::LEFT;
      }
    } else {
      if (y > 0) {
        return Direction::DOWN;
      } else {
        return Direction::UP;
      }
    }
  }

  // operator overloads

  Vector2 operator+(const Vector2 &other) const {
    return Vector2(x + other.x, y + other.y);
  }

  Vector2 operator-(const Vector2 &other) const {
    return Vector2(x - other.x, y - other.y);
  }

  Vector2 operator*(const Vector2 &other) const {
    return Vector2(x * other.x, y * other.y);
  }

  Vector2 operator/(const Vector2 &other) const {
    return Vector2(x / other.x, y / other.y);
  }

  Vector2 operator+(const T &other) const {
    return Vector2(x + other, y + other);
  }

  Vector2 operator-(const T &other) const {
    return Vector2(x - other, y - other);
  }

  Vector2 operator*(const T &other) const {
    return Vector2(x * other, y * other);
  }

  Vector2 operator/(const T &other) const {
    return Vector2(x / other, y / other);
  }

  Vector2 operator+=(const Vector2 &other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  Vector2 operator-=(const Vector2 &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Vector2 operator*=(const Vector2 &other) {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  Vector2 operator/=(const Vector2 &other) {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  Vector2 operator+=(const T &other) {
    x += other;
    y += other;
    return *this;
  }

  Vector2 operator-=(const T &other) {
    x -= other;
    y -= other;
    return *this;
  }

  Vector2 operator*=(const T &other) {
    x *= other;
    y *= other;
    return *this;
  }

  Vector2 operator/=(const T &other) {
    x /= other;
    y /= other;
    return *this;
  }

  bool operator==(const Vector2 &other) const {
    return x == other.x && y == other.y;
  }

  bool operator!=(const Vector2 &other) const {
    return x != other.x || y != other.y;
  }

  bool operator<(const Vector2 &other) const {
    return x < other.x && y < other.y;
  }

  bool operator>(const Vector2 &other) const {
    return x > other.x && y > other.y;
  }

  bool operator<=(const Vector2 &other) const {
    return x <= other.x && y <= other.y;
  }

  bool operator>=(const Vector2 &other) const {
    return x >= other.x && y >= other.y;
  }
};

// typedefs for common types
typedef Vector2<int> Vector2i;
typedef Vector2<long> Vector2l;
typedef Vector2<long long> Vector2ll;

typedef Vector2<unsigned int> Vector2u;
typedef Vector2<uint16_t> Vector2u_16;
typedef Vector2<uint32_t> Vector2u_32;
typedef Vector2<uint64_t> Vector2u_64;

typedef Vector2<int16_t> Vector2_16;
typedef Vector2<int32_t> Vector2_32;
typedef Vector2<int64_t> Vector2_64;

typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
