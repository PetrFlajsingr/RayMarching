//
// Created by petr on 4/4/20.
//

#ifndef RAYMARCHING_CSGSHAPES_H
#define RAYMARCHING_CSGSHAPES_H

#include "CSGCommon.h"
#include "SignedDistanceFunctions.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shape : public CSGRawData {
public:
  explicit Shape(const glm::vec3 &position) : position(position) {}
  glm::vec3 position;
  [[nodiscard]] virtual auto getName() const -> std::string = 0;
  [[nodiscard]] virtual auto distance(const glm::vec3 &camPos) const -> float = 0;
  int materialIndex = 0;

protected:
  [[nodiscard]] auto getMaterialFlag() const -> uint32_t;
};

struct BoxShape : public Shape {
  BoxShape(const glm::vec3 &position, const glm::vec3 &dimensions);
  glm::vec3 dimensions;
  [[nodiscard]] auto getName() const -> std::string override;
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto distance(const glm::vec3 &camPos) const -> float override;

  [[nodiscard]] auto rawTypeInfo() const -> uint32_t override;
  [[nodiscard]] auto rawParameters() const -> std::vector<float> override;
};

struct SphereShape : public Shape {
  SphereShape(const glm::vec3 &position, float radius);
  float radius;
  [[nodiscard]] auto getName() const -> std::string override;
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto distance(const glm::vec3 &camPos) const -> float override;

  [[nodiscard]] auto rawTypeInfo() const -> uint32_t override;
  [[nodiscard]] auto rawParameters() const -> std::vector<float> override;
};

class PlaneShape : public Shape {
public:
  PlaneShape(const glm::vec3 &position, const glm::vec4 &normal);
  [[nodiscard]] auto getName() const -> std::string override;
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto getNormal() const -> const glm::vec4 &;
  auto setNormal(const glm::vec4 &normal);
  [[nodiscard]] auto distance(const glm::vec3 &camPos) const -> float override;

  [[nodiscard]] auto rawTypeInfo() const -> uint32_t override;
  [[nodiscard]] auto rawParameters() const -> std::vector<float> override;

private:
  glm::vec4 normal;
};

template <C_Shape T> constexpr auto flagForShape() -> uint8_t {
  if constexpr (std::is_same_v<T, BoxShape>) {
    return 0b00000000;
  } else if constexpr (std::is_same_v<T, SphereShape>) {
    return flagForShape<BoxShape>() + 0b00000001;
  } else if constexpr (std::is_same_v<T, PlaneShape>) {
    return flagForShape<SphereShape>() + 0b00000001;
  }
}

template <C_Shape T> constexpr auto sdfForShape() {
  if constexpr (std::is_same_v<T, BoxShape>) {
    return sdf::box;
  } else if constexpr (std::is_same_v<T, SphereShape>) {
    return sdf::sphere;
  } else if constexpr (std::is_same_v<T, PlaneShape>) {
    return sdf::plane;
  }
}
#endif // RAYMARCHING_CSGSHAPES_H
