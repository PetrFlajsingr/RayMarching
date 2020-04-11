//
// Created by petr on 4/11/20.
//

#ifndef RAYMARCHING_RAWCONVERSION_H
#define RAYMARCHING_RAWCONVERSION_H
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
template <std::size_t DataSize, typename T> inline auto memToBytes(const T &val) -> std::array<uint8_t, DataSize> {
  auto raw = reinterpret_cast<uint8_t const *>(&val);
  std::array<uint8_t, DataSize> result;
  for (std::size_t i = 0; i < DataSize; ++i) {
    result[i] = raw[i];
  }
  return result;
}
inline auto vec3ToBytes(const glm::vec3 &vec) -> std::array<uint8_t, 12> { return memToBytes<12>(vec); }
inline auto vec4ToBytes(const glm::vec4 &vec) -> std::array<uint8_t, 16> { return memToBytes<16>(vec); }
inline auto floatToBytes(float val) -> std::array<uint8_t, 4> { return memToBytes<4>(val); }
inline auto intToBytes(int val) -> std::array<uint8_t, 4> { return memToBytes<4>(val); }
#pragma clang diagnostic pop

#endif // RAYMARCHING_RAWCONVERSION_H
