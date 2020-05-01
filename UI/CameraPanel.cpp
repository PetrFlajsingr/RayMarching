//

#include "CameraPanel.h"
#include <imgui/imgui.h>

//
// Created by petr on 4/3/20.
ui::CameraPanel::CameraPanel(Camera &camera) : camera(camera) {}

void ui::CameraPanel::onFrame() {
  ImGui::Begin("Camera");
  ImGui::Text("Distance: %.2f", distance);
  ImGui::Checkbox("Clip", &enableClipping);
  ImGui::SliderFloat("Speed", &movementSpeed, 0.1f, 50.f);
  ImGui::Text("Position %.2f %.2f %.2f", camera.Position.x, camera.Position.y, camera.Position.z);
  ImGui::End();
}
auto ui::CameraPanel::setDistance(float distance) -> void { CameraPanel::distance = distance; }
auto ui::CameraPanel::isClippingEnabled() const -> bool { return enableClipping; }
auto ui::CameraPanel::getMovementSpeed() const -> float { return movementSpeed; }
