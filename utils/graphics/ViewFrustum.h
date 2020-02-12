//
// Created by Petr on 19.11.2019.
//

#ifndef UTILITIES_VIEWFRUSTUM_H
#define UTILITIES_VIEWFRUSTUM_H

#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "common.h"
#include <array>
#include <glm/glm.hpp>

namespace geo {

class ViewFrustum {
  std::array<glm::vec4, 6> planes;

public:
  static ViewFrustum FromProjectionView(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);

  [[nodiscard]] RelativePosition contains(const BoundingBox<3> &aabb) const;
  [[nodiscard]] RelativePosition contains(const BoundingSphere<3> &bs) const;
};
} // namespace geo

#endif // UTILITIES_VIEWFRUSTUM_H
