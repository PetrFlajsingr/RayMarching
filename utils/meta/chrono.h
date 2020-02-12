//
// Created by Petr on 18.11.2019.
//

#ifndef UTILITIES_CHRONO_H
#define UTILITIES_CHRONO_H

#include "specializations.h"
#include <chrono>

template <typename T> static inline constexpr bool is_duration_v = is_specialization<T, std::chrono::duration>::value;

template <typename Duration> constexpr auto durationToString() {
  using namespace std::chrono;
  static_assert(is_duration_v<Duration>);
  if constexpr (std::is_same_v<Duration, nanoseconds>) {
    return "ns";
  } else if constexpr (std::is_same_v<Duration, microseconds>) {
    return "us";
  } else if constexpr (std::is_same_v<Duration, milliseconds>) {
    return "ms";
  } else if constexpr (std::is_same_v<Duration, seconds>) {
    return "s";
  } else if constexpr (std::is_same_v<Duration, minutes>) {
    return "m";
  } else if constexpr (std::is_same_v<Duration, hours>) {
    return "h";
  }
  return "";
}
#endif // UTILITIES_CHRONO_H
