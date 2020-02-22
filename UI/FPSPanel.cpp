//
// Created by petr on 2/22/20.
//

#include "FPSPanel.h"
#include <imgui/imgui.h>
#include <io/print.h>

void ui::FPSPanel::setVisible(bool visible) noexcept { FPSPanel::visible = visible; }

bool ui::FPSPanel::isVisible() const noexcept { return visible; }

void ui::FPSPanel::onFrame() {
  fpsCounter.frame();
  ImGui::Begin("FPS", &visible);
  ImGui::Text("FPS (avg): %.2f\nFPS (cur): %.2f", fpsCounter.average(), fpsCounter.current());
  if (ImGui::Button("Reset")) {
    reset();
  }
  ImGui::End();
  print(fpsCounter.average());
}

void ui::FPSPanel::reset() { fpsCounter.restart(); }
