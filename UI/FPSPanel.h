//
// Created by petr on 2/22/20.
//

#ifndef RAYMARCHING_FPSPANEL_H
#define RAYMARCHING_FPSPANEL_H
#include "Panel.h"
#include <time/FPSCounter.h>

namespace ui {
class FPSPanel : public Panel {
public:
  FPSPanel() = default;
  FPSPanel(FPSPanel &&other) = delete;
  FPSPanel(const FPSPanel &other) = delete;
  auto operator=(FPSPanel &&other) -> FPSPanel & = delete;
  auto operator=(const FPSPanel &other) -> FPSPanel & = delete;

  [[nodiscard]] auto getFPSCounter() const -> const FPSCounter &;

  auto onFrame() -> void override;
  auto reset() -> void;

private:
  FPSCounter fpsCounter;
};
} // namespace ui
#endif // RAYMARCHING_FPSPANEL_H
