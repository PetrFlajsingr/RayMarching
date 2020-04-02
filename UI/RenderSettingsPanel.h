//
// Created by petr on 2/23/20.
//

#ifndef RAYMARCHING_RENDERSETTINGSPANEL_H
#define RAYMARCHING_RENDERSETTINGSPANEL_H
#include "Panel.h"
#include <string>
#include <vector>

namespace ui {
enum class TextureChoice { Render, StepCount, Depth };
class RenderSettingsPanel : public Panel {
public:
  RenderSettingsPanel();

  auto onFrame() -> void override;

  [[nodiscard]] auto getSelectedTextureType() const -> TextureChoice;

private:
  const char *currentItem;
  std::vector<std::string> textureChoiceItems;
};
} // namespace ui
#endif // RAYMARCHING_RENDERSETTINGSPANEL_H
