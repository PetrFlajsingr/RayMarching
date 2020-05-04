//
// Created by petr on 4/11/20.
//

#include "MaterialManager.h"
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>

auto MaterialManager::addMaterial(Material &&material) -> void {
  const auto name = material.getName();
  materials[material.getName()] = std::move(material);
  materialsInOrder.emplace_back(&materials[name]);
}
auto MaterialManager::getMaterial(const std::string &name) -> Material & { return materials[name]; }
auto MaterialManager::getMaterial(const std::string &name) const -> const Material & { return materials[name]; }

auto MaterialManager::updateSSBO() -> void {
  if (std::any_of(materialsInOrder.begin(), materialsInOrder.end(), [](const auto &item) { return item->wasModified; })) {
    std::vector<uint8_t> data;
    data.reserve(materialsInOrder.size() * Material::paddedSize);
    for (auto &material : materialsInOrder) {
      const auto rawMaterial = material->raw();
      for (auto byte : rawMaterial) {
        data.emplace_back(byte);
      }
    }
    std::for_each(materialsInOrder.begin(), materialsInOrder.end(), [](auto &item) { return item->wasModified = false; });
    buffer.setData(data);
  }
}
auto MaterialManager::getMaterialBuffer() const -> const ge::gl::Buffer & { return buffer; }
auto MaterialManager::bindBuffer(unsigned int binding) -> void { buffer.bindBase(GL_SHADER_STORAGE_BUFFER, binding); }
auto MaterialManager::getMaterialMap() -> std::unordered_map<std::string, Material> & { return materials; }
auto MaterialManager::loadFromJson(const nlohmann::json &json) -> void {
  using namespace std::string_literals;
  if (!json.is_array()) {
    return;
  }
  for (const auto &item : json) {
    if (!item.contains("name") || !item.contains("type") || !item.contains("color")) {
      spdlog::error("Invalid definition of material: "s + item.dump(2));
      continue;
    }
    const auto materialType = magic_enum::enum_cast<Material::Type>(std::string(item["type"]));
    if (!materialType.has_value()) {
      spdlog::error("Invalid definition of material type: "s + item.dump(2));
      continue;
    }
    if (!item["color"].contains("r") || !item["color"].contains("g") || !item["color"].contains("b")) {
      spdlog::error("Invalid definition of material color: "s + item.dump(2));
      continue;
    }
    auto material = Material{item["name"], materialType.value()};
    const auto materialColor = glm::vec3{item["color"]["r"], item["color"]["g"], item["color"]["b"]};
    material.setColor(materialColor);
    if (item.contains("reflectivity")) {
      material.setReflectivity(item["reflectivity"]);
    }
    if (item.contains("refractiveIndex")) {
      material.setRefractiveIndex(item["refractiveIndex"]);
    }
    if (item.contains("refractiveFactor")) {
      material.setRefractiveFactor(item["refractiveFactor"]);
    }
    if (item.contains("scatterDensity")) {
      material.setScatterDensity(item["scatterDensity"]);
    }
    spdlog::info("Loaded material: {}", material);
    addMaterial(std::move(material));
  }
}
MaterialManager::MaterialManager() : buffer(maxMats * Material::paddedSize) {}
auto MaterialManager::getMaterialIndex(const std::string &name) const -> int {
  int cnt = 0;
  for (auto &material : materialsInOrder) {
    if (material->getName() == name) {
      break;
    }
    ++cnt;
  }
  return cnt;
}
