//
// Created by petr on 10/28/19.
//

#ifndef UTILITIES_NOW_H
#define UTILITIES_NOW_H
#include "../meta/chrono.h"
#include <chrono>

template <typename T, typename = std::enable_if_t<is_duration_v<T>>> inline auto now() {
  using namespace std::chrono;
  return duration_cast<T>(system_clock::now().time_since_epoch());
}

#endif // UTILITIES_NOW_H
