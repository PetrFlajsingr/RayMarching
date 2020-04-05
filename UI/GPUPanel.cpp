//
// Created by petr on 2/22/20.
//

#include "GPUPanel.h"

auto ui::GPUPanel::onFrame() -> void {
  if (isVisible()) {
    ImGui::Begin("GPU usage");
    const auto [usedMemory, availableMemory] = getGPUMemoryUsage();
    ImGui::Text("Memory total: %u\nMemory unused: %u", availableMemory, usedMemory);
    ImGui::End();
  }
}
