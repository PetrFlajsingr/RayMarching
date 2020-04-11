//
// Created by petr on 2/23/20.
//

#ifndef RAYMARCHING_RENDERSETTINGSPANEL_H
#define RAYMARCHING_RENDERSETTINGSPANEL_H
#include "../ray_marching/RayMarcher.h"
#include "Panel.h"
#include <functional>
#include <string>
#include <vector>
namespace ui {
class RenderSettingsPanel : public Panel {
public:
  RenderSettingsPanel();

  auto onFrame() -> void override;

  template <typename F> auto setOnReloadShaderClicked(F &&onClicked) { onReloadShaderClicked = onClicked; }

  [[nodiscard]] auto getSelectedTextureType() const -> ray_march::Tex;
  [[nodiscard]] auto getShadowType() const -> ray_march::Shadows;
  [[nodiscard]] auto getAAType() const -> ray_march::AntiAliasing;
  [[nodiscard]] auto getRayStepLimit() const -> int;
  [[nodiscard]] auto getShadowRayStepLimit() const -> int;
  [[nodiscard]] auto getMaxDrawDistance() const -> float;
  [[nodiscard]] auto getTimeScale() const -> float;
  [[nodiscard]] auto isAmbientOcclusionEnabled() const -> bool;
  [[nodiscard]] auto getAA() const -> int;
  [[nodiscard]] auto getMaxReflections() const -> int;
  [[nodiscard]] auto getLightPosition() const -> const glm::vec3 &;

private:
  const char *currentItem;
  const char *currentShadowItem;
  const char *currentAAItem;
  std::vector<std::string> textureChoiceItems;
  std::vector<std::string> shadowChoiceItems;
  std::vector<std::string> aaChoiceItems;
  int rayStepLimit = 512;
  int shadowRayStepLimit = 64;
  float maxDrawDistance = 10000.0f;
  float timeScale = 1.0f;
  int aaX = 2;
  int maxReflections = 1;
  bool ambientOcclusionEnabled = false;
  glm::vec3 lightPosition;
  std::function<void()> onReloadShaderClicked;
};
} // namespace ui
#endif // RAYMARCHING_RENDERSETTINGSPANEL_H
