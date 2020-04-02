//
// Created by petr on 2/23/20.
//

#include "RenderSettingsPanel.h"
#include <imgui/imgui.h>
#include <magic_enum.hpp>

ui::RenderSettingsPanel::RenderSettingsPanel() {
  for (auto val : magic_enum::enum_values<TextureChoice>()) {
    textureChoiceItems.emplace_back(magic_enum::enum_name(val));
  }
  currentItem = textureChoiceItems[0].data();
}

auto ui::RenderSettingsPanel::onFrame() -> void {
  if (isVisible()) {
    ImGui::Begin("Render settings");
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
    ImGui::SliderInt("Max ray steps", &rayStepLimit, 1, 512);
    ImGui::SliderFloat("Max draw distance", &maxDrawDistance, 0.1f, 1000.f);
    ImGui::End();
  }
}

auto ui::RenderSettingsPanel::getSelectedTextureType() const -> ui::TextureChoice {
  return magic_enum::enum_cast<TextureChoice>(currentItem).value();
}

auto ui::RenderSettingsPanel::getRayStepLimit() const -> int { return rayStepLimit; }
auto ui::RenderSettingsPanel::getMaxDrawDistance() const -> float { return maxDrawDistance; }
