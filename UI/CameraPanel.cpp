//
// Created by petr on 4/3/20.
//

#include "CameraPanel.h"
#include <imgui/imgui.h>

void ui::CameraPanel::onFrame() {
  if (isVisible()) {
    ImGui::Begin("Camera");
    ImGui::Text("Distance: %.2f", distance);
    ImGui::Checkbox("Clip", &enableClipping);
    ImGui::End();
  }
}
auto ui::CameraPanel::setDistance(float distance) -> void { CameraPanel::distance = distance; }
auto ui::CameraPanel::isClippingEnabled() const -> bool { return enableClipping; }
