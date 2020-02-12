//
// Created by Petr on 19.11.2019.
//

#include "Geometry.h"
#include <glm/geometric.hpp>

glm::vec3 geo::midPoint(const glm::vec3 &p1, const glm::vec3 &p2) {
  return glm::vec3{(p1.x + p2.x) / 2, (p1.y + p2.y) / 2, (p1.z + p2.z) / 2};
}

float geo::distanceToPlane(const glm::vec4 &plane, const glm::vec4 &point) { return glm::dot(point, plane); }
