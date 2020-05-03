//
// Created by petr on 4/4/20.
//

#include "CSGShapes.h"
#include <fmt/format.h>

BoxShape::BoxShape(const glm::vec3 &position, const glm::vec3 &dimensions) : Shape(position), dimensions(dimensions) {}

auto BoxShape::getName() const -> std::string { return "box"; }
auto BoxShape::src() const -> std::string {
  const auto positionStr = fmt::format("vec3({}, {}, {})", position.x, position.y, position.z);
  const auto dimensionsStr = fmt::format("vec3({}, {}, {})", dimensions.x, dimensions.y, dimensions.z);
  return fmt::format("sdBox(camPos - {}, {})", positionStr, dimensionsStr);
}
auto BoxShape::distance(const glm::vec3 &camPos) const -> float { return sdfForShape<BoxShape>()(camPos - position, dimensions); }
auto BoxShape::rawTypeInfo() const -> uint32_t {
  return flagForShape<std::decay_t<decltype(*this)>>() | rawCategory<std::decay_t<decltype(*this)>>() | getMaterialFlag();
}
auto BoxShape::rawParameters() const -> std::vector<float> {
  return std::vector<float>{position.x, position.y, position.z, dimensions.x, dimensions.y, dimensions.z};
}

SphereShape::SphereShape(const glm::vec3 &position, float radius) : Shape(position), radius(radius) {}

auto SphereShape::getName() const -> std::string { return "sphere"; }
auto SphereShape::src() const -> std::string {
  const auto positionStr = fmt::format("vec3({}, {}, {})", position.x, position.y, position.z);
  return fmt::format("sdSphere(camPos - {}, {})", positionStr, radius);
}
auto SphereShape::distance(const glm::vec3 &camPos) const -> float {
  return sdfForShape<SphereShape>()(camPos - position, radius);
}
auto SphereShape::rawTypeInfo() const -> uint32_t {
  return flagForShape<std::decay_t<decltype(*this)>>() | rawCategory<std::decay_t<decltype(*this)>>() | getMaterialFlag();
}
auto SphereShape::rawParameters() const -> std::vector<float> {
  return std::vector<float>{position.x, position.y, position.z, radius};
}

PlaneShape::PlaneShape(const glm::vec3 &position, const glm::vec4 &normal) : Shape(position), normal(glm::normalize(normal)) {}
auto PlaneShape::getNormal() const -> const glm::vec4 & { return normal; }
auto PlaneShape::setNormal(const glm::vec4 &normal) { PlaneShape::normal = glm::normalize(normal); }

auto PlaneShape::getName() const -> std::string { return "plane"; }
auto PlaneShape::src() const -> std::string {
  const auto positionStr = fmt::format("vec3({}, {}, {})", position.x, position.y, position.z);
  const auto normalStr = fmt::format("vec4({}, {}, {}, {})", normal.x, normal.y, normal.z, normal.w);
  return fmt::format("sdPlane(camPos - {}, {})", positionStr, normalStr);
}
auto PlaneShape::distance(const glm::vec3 &camPos) const -> float { return sdfForShape<PlaneShape>()(camPos - position, normal); }
auto PlaneShape::rawTypeInfo() const -> uint32_t {
  return flagForShape<std::decay_t<decltype(*this)>>() | rawCategory<std::decay_t<decltype(*this)>>() | getMaterialFlag();
}
auto PlaneShape::rawParameters() const -> std::vector<float> {
  return std::vector<float>{position.x, position.y, position.z, normal.x, normal.y, normal.z, normal.w};
}
auto Shape::getMaterialFlag() const -> uint32_t {
  constexpr uint32_t materialShift = 26;
  return uint32_t(materialIndex) << materialShift;
}
