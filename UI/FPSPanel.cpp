//
// Created by petr on 2/22/20.
//

#include "FPSPanel.h"
#include <imgui/imgui.h>

auto ui::FPSPanel::onFrame() -> void {
  fpsCounter.frame();
  if (isVisible()) {
    ImGui::Begin("FPS");
    ImGui::Text("FPS (avg): %.2f\nFPS (cur): %.2f", fpsCounter.average(), fpsCounter.current());
    ImGui::Checkbox("VSync", &vsyncEnabled);
    if (ImGui::Button("Reset")) {
      reset();
    }
    ImGui::End();
  }
}

auto ui::FPSPanel::reset() -> void { fpsCounter.restart(); }

auto ui::FPSPanel::getFPSCounter() const -> const FPSCounter & { return fpsCounter; }
auto ui::FPSPanel::isVsyncEnabled() const -> bool { return vsyncEnabled; }
