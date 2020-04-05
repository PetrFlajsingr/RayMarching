//
// Created by petr on 4/4/20.
//

#include "CSGShapes.h"
#include <fmt/format.h>

auto BoxShape::getRaw() const -> std::vector<uint8_t> {
  const auto positionRaw = vec3ToBytes(position);
  const auto dimensionsRaw = vec3ToBytes(dimensions);
  std::vector<uint8_t> result = {shapeFlag | flagForShape<BoxShape>()};
  for (auto byte : positionRaw) {
    result.emplace_back(byte);
  }
  for (auto byte : dimensionsRaw) {
    result.emplace_back(byte);
  }
  return result;
}
auto BoxShape::getDataSize() const -> std::size_t { return 25; }
auto BoxShape::getName() const -> std::string { return "box"; }
auto BoxShape::src() const -> std::string {
  const auto positionStr = fmt::format("vec3({}, {}, {})", position.x, position.y, position.z);
  const auto dimensionsStr = fmt::format("vec3({}, {}, {})", dimensions.x, dimensions.y, dimensions.z);
  return fmt::format("vec2(sdBox(camPos - {}, {}))", positionStr, dimensionsStr);
}

auto SphereShape::getRaw() const -> std::vector<uint8_t> {
  const auto positionRaw = vec3ToBytes(position);
  const auto radiusRaw = floatToBytes(radius);
  std::vector<uint8_t> result = {shapeFlag | flagForShape<BoxShape>()};
  for (auto byte : positionRaw) {
    result.emplace_back(byte);
  }
  for (auto byte : radiusRaw) {
    result.emplace_back(byte);
  }
  return result;
}
auto SphereShape::getDataSize() const -> std::size_t { return 17; }
auto SphereShape::getName() const -> std::string { return "sphere"; }
auto SphereShape::src() const -> std::string {
  const auto positionStr = fmt::format("vec3({}, {}, {})", position.x, position.y, position.z);
  return fmt::format("vec2(sdSphere(camPos - {}, {}))", positionStr, radius);
}
auto PlaneShape::getNormal() const -> const glm::vec3 & { return normal; }
auto PlaneShape::setNormal(const glm::vec3 &normal) { PlaneShape::normal = glm::normalize(normal); }
auto PlaneShape::getRaw() const -> std::vector<uint8_t> {
  const auto positionRaw = vec3ToBytes(position);
  const auto normalRaw = vec3ToBytes(normal);
  std::vector<uint8_t> result = {shapeFlag | flagForShape<BoxShape>()};
  for (auto byte : positionRaw) {
    result.emplace_back(byte);
  }
  for (auto byte : normalRaw) {
    result.emplace_back(byte);
  }
  return result;
}
auto PlaneShape::getDataSize() const -> std::size_t { return 25; }
auto PlaneShape::getName() const -> std::string { return "plane"; }
auto PlaneShape::src() const -> std::string {
  const auto positionStr = fmt::format("vec3({}, {}, {})", position.x, position.y, position.z);
  const auto normalStr = fmt::format("vec3({}, {}, {})", normal.x, normal.y, normal.z);
  return fmt::format("vec2(sdPlane(camPos - {}, {}))", positionStr, normalStr);
}
