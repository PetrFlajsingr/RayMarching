//
// Created by petr on 4/3/20.
//

#ifndef RAYMARCHING_CAMERAPANEL_H
#define RAYMARCHING_CAMERAPANEL_H

#include "Panel.h"
#include <Camera.h>
namespace ui {
class CameraPanel : public Panel {
public:
  explicit CameraPanel(Camera &camera);
  void onFrame() override;
  auto setDistance(float distance) -> void;
  [[nodiscard]] auto isClippingEnabled() const -> bool;
  [[nodiscard]] auto getMovementSpeed() const -> float;

private:
  Camera &camera;
  float distance = 0;
  bool enableClipping = true;
  float movementSpeed = 1.0f;
};
} // namespace ui

#endif // RAYMARCHING_CAMERAPANEL_H
