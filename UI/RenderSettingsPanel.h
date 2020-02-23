//
// Created by petr on 2/23/20.
//

#ifndef RAYMARCHING_RENDERSETTINGSPANEL_H
#define RAYMARCHING_RENDERSETTINGSPANEL_H
#include "Panel.h"
#include <vector>
#include <string>

namespace ui {
enum class TextureChoice {
  Render, StepCount, Depth
};
class RenderSettingsPanel : public Panel {
public:
  RenderSettingsPanel();

  void onFrame() override;

private:
  const char *currentItem;
  std::vector<std::string> textureChoiceItems;
};
}
#endif // RAYMARCHING_RENDERSETTINGSPANEL_H
