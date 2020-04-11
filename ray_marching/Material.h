//
// Created by petr on 4/11/20.
//

#ifndef RAYMARCHING_MATERIAL_H
#define RAYMARCHING_MATERIAL_H

#include <array>
#include <glm/glm.hpp>
#include <ostream>
#include <string>

class MaterialManager;
class Material {
  friend class MaterialManager;

public:
  static inline constexpr std::size_t paddedSize = 48;
  enum class Type : int { Normal = 0, Reflective, Transparent, Scatter };
  Material() = default;
  Material(std::string name, Type type);

  [[nodiscard]] auto getColor() const -> const glm::vec3 &;
  auto setColor(const glm::vec3 &color) -> void;
  [[nodiscard]] auto getReflectivity() const -> float;
  auto setReflectivity(float reflectivity) -> void;
  [[nodiscard]] auto getRefractiveIndex() const -> float;
  auto setRefractiveIndex(float refractiveIndex) -> void;
  [[nodiscard]] auto getRefractiveFactor() const -> float;
  auto setRefractiveFactor(float refractiveFactor) -> void;
  [[nodiscard]] auto getScatterDensity() const -> float;
  auto setScatterDensity(float scatterDensity) -> void;
  [[nodiscard]] auto getType() const -> Type;
  [[nodiscard]] auto getName() const -> const std::string &;
  auto setType(Type type) -> void;
  [[nodiscard]] auto raw() const -> std::array<uint8_t, paddedSize>;

  friend auto operator<<(std::ostream &ostream, const Material &material) -> std::ostream &;

private:
  Type type;
  glm::vec3 color{0};
  float reflectivity = 0;
  float refractiveIndex = 1;
  float refractiveFactor = 0;
  float scatterDensity = 0;
  std::string name;

  bool wasModified = true;
};

#endif // RAYMARCHING_MATERIAL_H
