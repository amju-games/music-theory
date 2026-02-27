// "microlib" minimal C++ game utility library
// (c) Copyright 2017-2022 Juliet Colman

#pragma once

#ifdef UNIT_TEST
#ifndef VEC2_DEBUG
#define VEC2_DEBUG
#endif
#endif

#ifdef VEC2_DEBUG
#include <iostream> 
#endif

#include <cassert>
#include <cmath>

struct vec2
{
  vec2() : x(0), y(0) {}
  vec2(float x_, float y_) : x(x_), y(y_) {}

  vec2& operator+=(const vec2& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  vec2& operator-=(const vec2& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  vec2& operator*=(float f)
  {
    x *= f;
    y *= f;
    return *this;
  }

  float x, y;
};

#ifdef VEC2_DEBUG
inline std::ostream& operator<<(std::ostream& os, const vec2& v)
{
  return os << "(" << v.x << ", " << v.y << ")";
}
#endif

inline bool operator==(const vec2& v1, const vec2& v2)
{
  return    v1.x == v2.x 
         && v1.y == v2.y;
} 

inline float dot(const vec2& v1, const vec2& v2)
{
  return v1.x * v2.x + v1.y * v2.y;
}

inline float squared_length(const vec2& v)
{
  return dot(v, v);
}

inline float length(const vec2& v)
{
  return sqrtf(squared_length(v));
}

inline vec2 operator*(const vec2& v, float f)
{
  return vec2(v.x * f, v.y * f);
}

inline vec2 operator*(float f, const vec2& v)
{
  return v * f;
}

inline vec2 operator+(const vec2& v1, const vec2& v2)
{
  return vec2(v1.x + v2.x, v1.y + v2.y);
}

inline vec2 operator-(const vec2& v1, const vec2& v2)
{
  return vec2(v1.x - v2.x, v1.y - v2.y);
}

inline vec2 operator-(const vec2& v)
{
  return vec2(-v.x, -v.y);
}

inline vec2 normalise(const vec2& v)
{
  float sqlen = squared_length(v);
  assert(sqlen > 0);
  float one_over = 1.0f / sqrtf(sqlen);
  return v * one_over;
}


