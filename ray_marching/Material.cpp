//
// Created by petr on 4/11/20.
//

#include "Material.h"
#include "RawConversion.h"
#include <magic_enum.hpp>
#include <types/Range.h>
using namespace MakeRange;

Material::Material(std::string name, Material::Type type) : type(type), name(std::move(name)) {}
auto Material::getColor() const -> const glm::vec3 & { return color; }
auto Material::setColor(const glm::vec3 &color) -> void {
  Material::color = color;
  wasModified = true;
}
auto Material::getReflectivity() const -> float { return reflectivity; }
auto Material::setReflectivity(float reflectivity) -> void {
  Material::reflectivity = reflectivity;
  wasModified = true;
}
auto Material::getRefractiveIndex() const -> float { return refractiveIndex; }
auto Material::setRefractiveIndex(float refractiveIndex) -> void {
  Material::refractiveIndex = refractiveIndex;
  wasModified = true;
}
auto Material::getRefractiveFactor() const -> float { return refractiveFactor; }
auto Material::setRefractiveFactor(float refractiveFactor) -> void {
  Material::refractiveFactor = refractiveFactor;
  wasModified = true;
}
auto Material::getScatterDensity() const -> float { return scatterDensity; }
auto Material::setScatterDensity(float scatterDensity) -> void {
  Material::scatterDensity = scatterDensity;
  wasModified = true;
}
auto operator<<(std::ostream &ostream, const Material &material) -> std::ostream & {
  return ostream << "[Material info]\n: "
                 << "Name: " << material.name << "\n"
                 << "Type: " << magic_enum::enum_name(material.type) << "\n"
                 << "Color: " << material.color.r << " " << material.color.g << " " << material.color.b << "\n"
                 << "Reflectivity: " << material.reflectivity << "\n"
                 << "Refractive index: " << material.refractiveIndex << "\n"
                 << "Refractive factor: " << material.refractiveFactor << "\n"
                 << "Scatter density: " << material.scatterDensity << "\n";
}
auto Material::raw() const -> std::array<uint8_t, paddedSize> {
  const auto typeRaw = intToBytes(static_cast<int>(type));
  const auto colorRaw = vec3ToBytes(color);
  const auto reflectivityRaw = floatToBytes(reflectivity);
  const auto refractiveIndexRaw = floatToBytes(refractiveIndex);
  const auto refractiveFactorRaw = floatToBytes(refractiveFactor);
  const auto scatterDensityRaw = floatToBytes(scatterDensity);
  std::array<uint8_t, paddedSize> result;
  std::size_t pos = 0;
  for (auto byte : colorRaw) {
    result[pos] = byte;
    ++pos;
  }
  for (auto i : range(4)) {
    result[pos] = 0;
    ++pos;
  }
  for (auto byte : typeRaw) {
    result[pos] = byte;
    ++pos;
  }
  for (auto byte : reflectivityRaw) {
    result[pos] = byte;
    ++pos;
  }
  for (auto byte : refractiveIndexRaw) {
    result[pos] = byte;
    ++pos;
  }
  for (auto byte : refractiveFactorRaw) {
    result[pos] = byte;
    ++pos;
  }
  for (auto byte : scatterDensityRaw) {
    result[pos] = byte;
    ++pos;
  }
  for (; pos < paddedSize; ++pos) {
    result[pos] = 0;
  }
  return result;
}
auto Material::getType() const -> Material::Type { return type; }
auto Material::getName() const -> const std::string & { return name; }
auto Material::setType(Material::Type type) -> void { Material::type = type; }
