//
// Created by petr on 4/4/20.
//

#ifndef RAYMARCHING_CSGSHAPES_H
#define RAYMARCHING_CSGSHAPES_H

#include "CSGCommon.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shape : public CSGRawData {
public:
  explicit Shape(const glm::vec3 &position) : position(position) {}
  glm::vec3 position;
  [[nodiscard]] virtual auto getName() const -> std::string = 0;
};

struct BoxShape : public Shape {
  BoxShape(const glm::vec3 &position, const glm::vec3 &dimensions) : Shape(position), dimensions(dimensions) {}
  glm::vec3 dimensions;
  [[nodiscard]] auto getRaw() -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() -> std::size_t override;
  [[nodiscard]] auto getName() const -> std::string override;
  [[nodiscard]] auto src() -> std::string override;
};

struct SphereShape : public Shape {
  SphereShape(const glm::vec3 &position, float radius) : Shape(position), radius(radius) {}
  float radius;
  [[nodiscard]] auto getRaw() -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() -> std::size_t override;
  [[nodiscard]] auto getName() const -> std::string override;
  [[nodiscard]] auto src() -> std::string override;
};

template <C_Shape T> constexpr uint8_t flagForShape() {
  if constexpr (std::is_same_v<T, BoxShape>) {
    return 0b00000000;
  } else if constexpr (std::is_same_v<T, SphereShape>) {
    return flagForShape<BoxShape>() + 0b00000001;
  }
  throw "invalid operation type";
}
#endif // RAYMARCHING_CSGSHAPES_H
