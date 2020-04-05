//
// Created by petr on 4/5/20.
//

#ifndef RAYMARCHING_SIGNEDDISTANCEFUNCTIONS_H
#define RAYMARCHING_SIGNEDDISTANCEFUNCTIONS_H

#include <glm/glm.hpp>

namespace sdf {
inline auto sphere(const glm::vec3 &p, float radius) -> float { return length(p) - radius; }

inline auto plane(const glm::vec3 &p, const glm::vec4 &n) -> float { return dot(p, n.xyz()) + n.w; }

inline auto box(const glm::vec3 &p, const glm::vec3 &b) -> float {
  glm::vec3 d = glm::abs(p) - b;
  return glm::min(glm::max(d.x, glm::max(d.y, d.z)), 0.0f) + glm::length(glm::max(d, 0.0f));
}
} // namespace sdf

#endif // RAYMARCHING_SIGNEDDISTANCEFUNCTIONS_H
