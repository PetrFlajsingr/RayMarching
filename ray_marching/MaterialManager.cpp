//
// Created by petr on 4/11/20.
//

#include "MaterialManager.h"

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
