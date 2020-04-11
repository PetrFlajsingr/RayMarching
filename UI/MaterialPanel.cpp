//
// Created by petr on 4/11/20.
//

#include "MaterialPanel.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <magic_enum.hpp>
#include <various/isin.h>

ui::MaterialPanel::MaterialPanel(MaterialManager &materialManager) : materialManager(materialManager) {
  for (auto val : magic_enum::enum_values<Material::Type>()) {
    materialTypeChoices.emplace_back(magic_enum::enum_name(val));
  }
}

void ui::MaterialPanel::onFrame() {
  ImGui::Begin("Materials");
  if (materialManager.getMaterialMap().size() != materials.size()) {
    materials.clear();
    for (auto &[name, _] : materialManager.getMaterialMap()) {
      materials.emplace_back(name);
    }
    currentMaterial = materials[currentMaterialindex].data();
    selectedMaterial = &materialManager.getMaterial(currentMaterial);
    currentMaterialType =
        std::find(materialTypeChoices.begin(), materialTypeChoices.end(), magic_enum::enum_name(selectedMaterial->getType()))
            ->data();
    color = selectedMaterial->getColor();
    reflectivity = selectedMaterial->getReflectivity();
    refractiveIndex = selectedMaterial->getRefractiveIndex();
    refractiveFactor = selectedMaterial->getRefractiveFactor();
    scatterDensity = selectedMaterial->getScatterDensity();
  }
  if (ImGui::BeginCombo("Material", currentMaterial)) {
    for (auto &choice : materials) {
      bool isSelected = (currentMaterial == choice.data());
      if (ImGui::Selectable(choice.data(), isSelected)) {
        currentMaterial = choice.data();
        selectedMaterial = &materialManager.getMaterial(currentMaterial);
        currentMaterialType =
            std::find(materialTypeChoices.begin(), materialTypeChoices.end(), magic_enum::enum_name(selectedMaterial->getType()))
                ->data();
        color = selectedMaterial->getColor();
        reflectivity = selectedMaterial->getReflectivity();
        refractiveIndex = selectedMaterial->getRefractiveIndex();
        refractiveFactor = selectedMaterial->getRefractiveFactor();
        scatterDensity = selectedMaterial->getScatterDensity();
      }
      if (isSelected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }

  if (ImGui::BeginCombo("Material type", currentMaterialType)) {
    for (auto &choice : materialTypeChoices) {
      bool isSelected = (currentMaterialType == choice.data());
      if (ImGui::Selectable(choice.data(), isSelected)) {
        currentMaterialType = choice.data();
        const auto oldMaterialType = selectedMaterial->getType();
        const auto selectedMaterialtype = magic_enum::enum_cast<Material::Type>(currentMaterialType).value();
        if (selectedMaterialtype != oldMaterialType) {
          selectedMaterial->setType(selectedMaterialtype);
        }
      }
      if (isSelected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
  const auto materialType = selectedMaterial->getType();
  ImGui::SliderFloat3("Color", glm::value_ptr(color), 0, 1);
  if (isIn(materialType, {Material::Type::Reflective, Material::Type::Transparent})) {
    ImGui::SliderFloat("Reflectivity", &reflectivity, 0, 1);
  }
  if (materialType == Material::Type::Transparent) {
    ImGui::SliderFloat("RefractiveIndex", &refractiveIndex, 0.1, 10);
    ImGui::SliderFloat("RefractiveFactor", &refractiveFactor, 0, 1);
  }
  if (materialType == Material::Type::Scatter) {
    ImGui::SliderFloat("Scatter density", &scatterDensity, 0.001, 1);
  }

  if (color != selectedMaterial->getColor()) {
    selectedMaterial->setColor(color);
  }
  if (reflectivity != selectedMaterial->getReflectivity()) {
    selectedMaterial->setReflectivity(reflectivity);
  }
  if (refractiveIndex != selectedMaterial->getRefractiveIndex()) {
    selectedMaterial->setRefractiveIndex(refractiveIndex);
  }
  if (refractiveFactor != selectedMaterial->getRefractiveFactor()) {
    selectedMaterial->setRefractiveFactor(refractiveFactor);
  }
  if (scatterDensity != selectedMaterial->getScatterDensity()) {
    selectedMaterial->setScatterDensity(scatterDensity);
  }

  ImGui::End();
}
