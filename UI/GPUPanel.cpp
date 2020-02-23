//
// Created by petr on 2/22/20.
//

#include "GPUPanel.h"

void ui::GPUPanel::onFrame() {
  if (isVisible()) {
    ImGui::Begin("GPU usage");
    const auto [usedMemory, availableMemory] = getGPUMemoryUsage();
    ImGui::Text("Memory total: %u\nMemory used: %u", availableMemory, usedMemory);
    ImGui::End();
  }
}
