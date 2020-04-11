//
// Created by petr on 4/4/20.
//

#ifndef RAYMARCHING_CSGCOMMON_H
#define RAYMARCHING_CSGCOMMON_H

#include "../RawConversion.h"
#include <array>
#include <glm/glm.hpp>
#include <vector>
class BinaryOperation;
class SpaceWarpOperation;
class Shape;

constexpr uint8_t operationFlag = 0b00000000;
constexpr uint8_t shapeFlag = 0b10000000;

template <typename T> concept C_Shape = std::is_base_of_v<Shape, T>;
template <typename T> concept C_Operation = std::is_base_of_v<BinaryOperation, T>;
template <typename T> concept C_WarpOperation = std::is_base_of_v<SpaceWarpOperation, T>;

struct CSGRawData {
  [[nodiscard]] virtual auto getRaw() const -> std::vector<uint8_t> = 0;
  [[nodiscard]] virtual auto getDataSize() const -> std::size_t = 0;
  [[nodiscard]] virtual auto src() const -> std::string = 0;
  virtual ~CSGRawData() = default;
};

inline auto replace(const std::string &str, const std::string &find, const std::string &replace) -> std::string {
  std::string result;
  std::size_t find_len = find.size();
  std::size_t pos, from = 0;
  if (std::string::npos != (pos = str.find(find, from))) {
    result.append(str, from, pos - from);
    result.append(replace);
    from = pos + find_len;
  }
  result.append(str, from, std::string::npos);
  return result;
}

#endif // RAYMARCHING_CSGCOMMON_H
