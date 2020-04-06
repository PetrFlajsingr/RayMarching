//
// Created by petr on 2/23/20.
//

#include "RenderSettingsPanel.h"
#include <imgui/imgui.h>
#include <magic_enum.hpp>

ui::RenderSettingsPanel::RenderSettingsPanel() {
  for (auto val : magic_enum::enum_values<ray_march::Tex>()) {
    textureChoiceItems.emplace_back(magic_enum::enum_name(val));
  }
  currentItem = textureChoiceItems[0].data();
  for (auto val : magic_enum::enum_values<ray_march::Shadows>()) {
    shadowChoiceItems.emplace_back(magic_enum::enum_name(val));
  }
  currentShadowItem = shadowChoiceItems[0].data();
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
    ImGui::SliderInt("Max ray steps", &rayStepLimit, 1, 2048);
    ImGui::SliderInt("Max shadow ray steps", &shadowRayStepLimit, 1, 512);
    ImGui::SliderFloat("Max draw distance", &maxDrawDistance, 0.1f, 10000.f);
    ImGui::SliderFloat("Time scaling", &timeScale, 0.f, 100.f);
    ImGui::Checkbox("Ambient occlusion", &ambientOcclusionEnabled);
    ImGui::Checkbox("FXAA", &enableFXAA);
    ImGui::Checkbox("SSAA", &antiAliasingEnabled);
    ImGui::SliderInt("SSAAx", &aaX, 1, 8);
    ImGui::Checkbox("Reflections", &reflectionsEnabled);
    ImGui::SliderInt("Max reflections", &maxReflections, 1, 10);
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
auto ui::RenderSettingsPanel::isAntiAliasingEnabled() const -> bool { return antiAliasingEnabled; }
auto ui::RenderSettingsPanel::getShadowType() const -> ray_march::Shadows {
  return magic_enum::enum_cast<ray_march::Shadows>(currentShadowItem).value();
}
auto ui::RenderSettingsPanel::getAA() const -> int { return aaX; }
auto ui::RenderSettingsPanel::getShadowRayStepLimit() const -> int { return shadowRayStepLimit; }
auto ui::RenderSettingsPanel::areReflectionsEnabled() const -> bool { return reflectionsEnabled; }
auto ui::RenderSettingsPanel::getMaxReflections() const -> int { return maxReflections; }
auto ui::RenderSettingsPanel::isFXAAEnabled() const -> bool { return enableFXAA; }
