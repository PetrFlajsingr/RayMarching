//
// Created by petr on 2/22/20.
//

#ifndef RAYMARCHING_FPSPANEL_H
#define RAYMARCHING_FPSPANEL_H
#include <time/FPSCounter.h>
#include "Panel.h"

namespace ui {
class FPSPanel : public Panel {
public:
  FPSPanel() = default;
  FPSPanel(FPSPanel &&other) = delete;
  FPSPanel(const FPSPanel &other) = delete;
  FPSPanel &operator=(FPSPanel &&other) = delete;
  FPSPanel &operator=(const FPSPanel &other) = delete;

  void onFrame() override;
  void reset();
private:
  bool visible = true;
  FPSCounter fpsCounter;
};
}
#endif // RAYMARCHING_FPSPANEL_H
