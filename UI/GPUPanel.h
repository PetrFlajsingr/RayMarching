//
// Created by petr on 2/22/20.
//

#ifndef RAYMARCHING_GPUPANEL_H
#define RAYMARCHING_GPUPANEL_H
#include "Panel.h"
#include <imgui/imgui.h>
#include "../common/gl_utils.h"

namespace ui {
class GPUPanel : public Panel {
public:
  GPUPanel() = default;
  GPUPanel(GPUPanel &&other) = delete;
  GPUPanel(const GPUPanel &other) = delete;
  GPUPanel &operator=(GPUPanel &&other) = delete;
  GPUPanel &operator=(const GPUPanel &other) = delete;

  void onFrame() override;

private:
};
}
#endif // RAYMARCHING_GPUPANEL_H
