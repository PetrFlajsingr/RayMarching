//
// Created by Petr on 19.11.2019.
//

#ifndef UTILITIES_GEOMETRY_H
#define UTILITIES_GEOMETRY_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace geo {
float distanceToPlane(const glm::vec4 &plane, const glm::vec4 &point);

glm::vec3 midPoint(const glm::vec3 &p1, const glm::vec3 &p2);
} // namespace geo

#endif // UTILITIES_GEOMETRY_H
