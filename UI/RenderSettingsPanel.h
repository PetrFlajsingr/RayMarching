//
// Created by petr on 2/23/20.
//

#ifndef RAYMARCHING_RENDERSETTINGSPANEL_H
#define RAYMARCHING_RENDERSETTINGSPANEL_H
#include "../ray_marching/RayMarcher.h"
#include "Panel.h"
#include <string>
#include <vector>
namespace ui {
class RenderSettingsPanel : public Panel {
public:
  RenderSettingsPanel();

  auto onFrame() -> void override;

  [[nodiscard]] auto getSelectedTextureType() const -> ray_march::Tex;
  [[nodiscard]] auto getShadowType() const -> ray_march::Shadows;
  [[nodiscard]] auto getRayStepLimit() const -> int;
  [[nodiscard]] auto getMaxDrawDistance() const -> float;
  [[nodiscard]] auto getTimeScale() const -> float;
  [[nodiscard]] auto isAmbientOcclusionEnabled() const -> bool;
  [[nodiscard]] auto isAntiAliasingEnabled() const -> bool;

private:
  const char *currentItem;
  const char *currentShadowItem;
  std::vector<std::string> textureChoiceItems;
  std::vector<std::string> shadowChoiceItems;
  int rayStepLimit = 64;
  float maxDrawDistance = 100.0f;
  float timeScale = 1.0f;
  bool ambientOcclusionEnabled = false;
  bool antiAliasingEnabled = false;
};
} // namespace ui
#endif // RAYMARCHING_RENDERSETTINGSPANEL_H
