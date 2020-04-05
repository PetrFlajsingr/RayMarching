//
// Created by petr on 4/5/20.
//

#ifndef RAYMARCHING_CSGOPERATIONSFUNCTIONS_H
#define RAYMARCHING_CSGOPERATIONSFUNCTIONS_H

#include <glm/glm.hpp>

namespace csg_op {
inline auto opMinus(float d1, float d2) -> float { return glm::max(-d2, d1); }

inline auto opUnion(float d1, float d2) -> float { return (d1 < d2) ? d1 : d2; }

inline auto sminCubic(float a, float b, float k) -> float {
  const auto h = glm::max(k - glm::abs(a - b), 0.0f);
  return glm::min(a, b) - h * h * h / (6.0f * k * k);
}

inline auto opBlend(float d1, float d2, float k) -> float {
  float d = sminCubic(d1, d2, k);
  return d;
}
} // namespace csg_op

#endif // RAYMARCHING_CSGOPERATIONSFUNCTIONS_H
