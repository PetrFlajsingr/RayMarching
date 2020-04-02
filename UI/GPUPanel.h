//
// Created by petr on 2/22/20.
//

#ifndef RAYMARCHING_GPUPANEL_H
#define RAYMARCHING_GPUPANEL_H
#include "../common/gl_utils.h"
#include "Panel.h"
#include <imgui/imgui.h>

namespace ui {
class GPUPanel : public Panel {
public:
  GPUPanel() = default;
  GPUPanel(GPUPanel &&other) = delete;
  GPUPanel(const GPUPanel &other) = delete;
  auto operator=(GPUPanel &&other) -> GPUPanel & = delete;
  auto operator=(const GPUPanel &other) -> GPUPanel & = delete;

  auto onFrame() -> void override;

private:
};
} // namespace ui
#endif // RAYMARCHING_GPUPANEL_H
