//
// Created by petr on 4/4/20.
//

#include "CSGShapes.h"
#include <fmt/format.h>

auto BoxShape::getRaw() -> std::vector<uint8_t> {
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
auto BoxShape::getDataSize() -> std::size_t { return 25; }
auto BoxShape::getName() const -> std::string { return "box"; }
auto BoxShape::src() -> std::string {
  const auto positionStr = fmt::format("vec3({}, {}, {})", position.x, position.y, position.z);
  const auto dimensionsStr = fmt::format("vec3({}, {}, {})", dimensions.x, dimensions.y, dimensions.z);
  return fmt::format("vec2(sdBox(camPos - {}, {}))", positionStr, dimensionsStr);
}

auto SphereShape::getRaw() -> std::vector<uint8_t> {
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
auto SphereShape::getDataSize() -> std::size_t { return 17; }
auto SphereShape::getName() const -> std::string { return "sphere"; }
auto SphereShape::src() -> std::string {
  const auto positionStr = fmt::format("vec3({}, {}, {})", position.x, position.y, position.z);
  return fmt::format("vec2(sdSphere(camPos - {}, {}))", positionStr, radius);
}