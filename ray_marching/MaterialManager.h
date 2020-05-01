//
// Created by petr on 4/11/20.
//

#ifndef RAYMARCHING_MATERIALMANAGER_H
#define RAYMARCHING_MATERIALMANAGER_H
#include "Material.h"
#include <geGL/Buffer.h>
#include <nlohmann/json.hpp>
#include <unordered_map>

class MaterialManager {
  static inline constexpr std::size_t maxMats = 100;

public:
  MaterialManager() : buffer(maxMats * Material::paddedSize) {}
  auto loadFromJson(const nlohmann::json &json) -> void;
  auto addMaterial(Material &&material) -> void;
  [[nodiscard]] auto getMaterial(const std::string &name) -> Material &;
  [[nodiscard]] auto getMaterial(const std::string &name) const -> const Material &;
  auto updateSSBO() -> void;
  [[nodiscard]] auto getMaterialBuffer() const -> const ge::gl::Buffer &;
  auto bindBuffer(unsigned int binding) -> void;
  [[nodiscard]] auto getMaterialMap() -> std::unordered_map<std::string, Material> &;

private:
  mutable std::unordered_map<std::string, Material> materials;
  mutable std::vector<Material *> materialsInOrder;
  ge::gl::Buffer buffer;
};

#endif // RAYMARCHING_MATERIALMANAGER_H
