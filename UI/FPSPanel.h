//
// Created by petr on 2/22/20.
//

#ifndef RAYMARCHING_FPSPANEL_H
#define RAYMARCHING_FPSPANEL_H
#include <time/FPSCounter.h>

namespace ui {
class FPSPanel {
public:
  FPSPanel() = default;

  void setVisible(bool visible) noexcept;
  [[nodiscard]] bool isVisible() const noexcept;
  void onFrame();
  void reset();
private:
  bool visible = true;
  FPSCounter fpsCounter;
};
}
#endif // RAYMARCHING_FPSPANEL_H
