//
// Created by petr on 4/4/20.
//

#ifndef RAYMARCHING_CSGCOMMON_H
#define RAYMARCHING_CSGCOMMON_H

#include <array>
#include <glm/glm.hpp>
#include <vector>
class Operation;
class Shape;

constexpr uint8_t operationFlag = 0b00000000;
constexpr uint8_t shapeFlag = 0b10000000;

template <typename T> concept C_Shape = std::is_base_of_v<Shape, T>;
template <typename T> concept C_Operation = std::is_base_of_v<Operation, T>;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
inline auto vec3ToBytes(const glm::vec3 &vec) -> std::array<uint8_t, 12> {
  auto raw = reinterpret_cast<uint8_t const *>(&vec);
  std::array<uint8_t, 12> result;
  for (uint8_t i = 0; i < 12; ++i) {
    result[i] = raw[i];
  }
  return result;
}
inline auto floatToBytes(float val) -> std::array<uint8_t, 4> {
  auto raw = reinterpret_cast<uint8_t const *>(&val);
  std::array<uint8_t, 4> result;
  for (uint8_t i = 0; i < 4; ++i) {
    result[i] = raw[i];
  }
  return result;
}
#pragma clang diagnostic pop

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
