//
// Created by petr on 2/23/20.
//

#include "RenderSettingsPanel.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <magic_enum.hpp>
#include <various/isin.h>

ui::RenderSettingsPanel::RenderSettingsPanel() {
  for (auto val : magic_enum::enum_values<ray_march::Tex>()) {
    textureChoiceItems.emplace_back(magic_enum::enum_name(val));
  }
  currentItem = textureChoiceItems[0].data();
  for (auto val : magic_enum::enum_values<ray_march::Shadows>()) {
    shadowChoiceItems.emplace_back(magic_enum::enum_name(val));
  }
  currentShadowItem = shadowChoiceItems[0].data();
  for (auto val : magic_enum::enum_values<ray_march::AntiAliasing>()) {
    aaChoiceItems.emplace_back(magic_enum::enum_name(val));
  }
  currentAAItem = aaChoiceItems[0].data();
}

auto ui::RenderSettingsPanel::onFrame() -> void {
  if (isVisible()) {
    ImGui::Begin("Render settings");
    if (ImGui::Button("Reload shader")) {
      onReloadShaderClicked();
    }
    if (ImGui::BeginCombo("Texture", currentItem)) {
      for (auto &choice : textureChoiceItems) {
        bool isSelected = (currentItem == choice.data());
        if (ImGui::Selectable(choice.data(), isSelected)) {
          currentItem = choice.data();
        }
        if (isSelected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
    ImGui::SliderInt("Max ray steps", &rayStepLimit, 1, 2048);
    ImGui::SliderFloat("Max draw distance", &maxDrawDistance, 1.0f, 100000.f);
    ImGui::SliderFloat("Time scaling", &timeScale, 0.f, 100.f);
    ImGui::Checkbox("Ambient occlusion", &ambientOcclusionEnabled);
    if (ImGui::BeginCombo("Shadows", currentShadowItem)) {
      for (auto &choice : shadowChoiceItems) {
        bool isSelected = (currentShadowItem == choice.data());
        if (ImGui::Selectable(choice.data(), isSelected)) {
          currentShadowItem = choice.data();
        }
        if (isSelected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
    const auto shadowType = getShadowType();
    if (isIn(shadowType, {ray_march::Shadows::Hard, ray_march::Shadows::Soft, ray_march::Shadows::RayTraced})) {
      ImGui::SliderInt("Max shadow ray steps", &shadowRayStepLimit, 1, 512);
    }
    if (ImGui::BeginCombo("Anti-aliasing", currentAAItem)) {
      for (auto &choice : aaChoiceItems) {
        bool isSelected = (currentAAItem == choice.data());
        if (ImGui::Selectable(choice.data(), isSelected)) {
          currentAAItem = choice.data();
        }
        if (isSelected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
    const auto aaType = getAAType();
    switch (aaType) {
    case ray_march::AntiAliasing::SSAA:
      ImGui::SliderInt("SSAAx", &aaX, 1, 8);
      break;
    default:
      break;
    }
    ImGui::SliderInt("Max reflections/refractions", &maxReflections, 0, 50);
    ImGui::SliderFloat3("Light direction", glm::value_ptr(lightPosition), -1, 1.0);
    lightPosition = glm::normalize(lightPosition);
    ImGui::End();
  }
}

auto ui::RenderSettingsPanel::getSelectedTextureType() const -> ray_march::Tex {
  return magic_enum::enum_cast<ray_march::Tex>(currentItem).value();
}

auto ui::RenderSettingsPanel::getRayStepLimit() const -> int { return rayStepLimit; }
auto ui::RenderSettingsPanel::getMaxDrawDistance() const -> float { return maxDrawDistance; }
auto ui::RenderSettingsPanel::getTimeScale() const -> float { return timeScale; }
auto ui::RenderSettingsPanel::isAmbientOcclusionEnabled() const -> bool { return ambientOcclusionEnabled; }
auto ui::RenderSettingsPanel::getShadowType() const -> ray_march::Shadows {
  return magic_enum::enum_cast<ray_march::Shadows>(currentShadowItem).value();
}
auto ui::RenderSettingsPanel::getAA() const -> int { return aaX; }
auto ui::RenderSettingsPanel::getShadowRayStepLimit() const -> int { return shadowRayStepLimit; }
auto ui::RenderSettingsPanel::getMaxReflections() const -> int { return maxReflections; }
auto ui::RenderSettingsPanel::getAAType() const -> ray_march::AntiAliasing {
  return magic_enum::enum_cast<ray_march::AntiAliasing>(currentAAItem).value();
}
auto ui::RenderSettingsPanel::getLightPosition() const -> const glm::vec3 & { return lightPosition; }
