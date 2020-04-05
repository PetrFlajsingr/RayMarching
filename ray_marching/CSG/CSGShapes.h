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
  [[nodiscard]] auto getRaw() const -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() const -> std::size_t override;
  [[nodiscard]] auto getName() const -> std::string override;
  [[nodiscard]] auto src() const -> std::string override;
};

struct SphereShape : public Shape {
  SphereShape(const glm::vec3 &position, float radius) : Shape(position), radius(radius) {}
  float radius;
  [[nodiscard]] auto getRaw() const -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() const -> std::size_t override;
  [[nodiscard]] auto getName() const -> std::string override;
  [[nodiscard]] auto src() const -> std::string override;
};

class PlaneShape : public Shape {
public:
  PlaneShape(const glm::vec3 &position, const glm::vec3 &normal) : Shape(position), normal(glm::normalize(normal)) {}
  [[nodiscard]] auto getRaw() const -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() const -> std::size_t override;
  [[nodiscard]] auto getName() const -> std::string override;
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto getNormal() const -> const glm::vec3 &;
  auto setNormal(const glm::vec3 &normal);

private:
  glm::vec3 normal;
};

template <C_Shape T> constexpr auto flagForShape() -> uint8_t {
  if constexpr (std::is_same_v<T, BoxShape>) {
    return 0b00000000;
  } else if constexpr (std::is_same_v<T, SphereShape>) {
    return flagForShape<BoxShape>() + 0b00000001;
  } else if constexpr (std::is_same_v<T, PlaneShape>) {
    return flagForShape<SphereShape>() + 0b00000001;
  }
  throw "invalid operation type";
}
#endif // RAYMARCHING_CSGSHAPES_H
