//
// Created by petr on 2/22/20.
//

#include "FPSPanel.h"
#include <imgui/imgui.h>


void ui::FPSPanel::onFrame() {
  fpsCounter.frame();
  if (isVisible()) {
    ImGui::Begin("FPS");
    ImGui::Text("FPS (avg): %.2f\nFPS (cur): %.2f", fpsCounter.average(), fpsCounter.current());
    if (ImGui::Button("Reset")) {
      reset();
    }
    ImGui::End();
  }
}

void ui::FPSPanel::reset() { fpsCounter.restart(); }
