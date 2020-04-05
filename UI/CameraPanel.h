//
// Created by petr on 4/3/20.
//

#ifndef RAYMARCHING_CAMERAPANEL_H
#define RAYMARCHING_CAMERAPANEL_H

#include "Panel.h"
namespace ui {
class CameraPanel : public Panel {
public:
  void onFrame() override;
  auto setDistance(float distance) -> void;
  [[nodiscard]] auto isClippingEnabled() const -> bool;

private:
  float distance = 0;
  bool enableClipping = true;
};
} // namespace ui

#endif // RAYMARCHING_CAMERAPANEL_H
