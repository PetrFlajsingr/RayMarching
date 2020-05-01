//
// Created by petr on 5/1/20.
//

#ifndef RAYMARCHING_SCENEPANEL_H
#define RAYMARCHING_SCENEPANEL_H
#include "../ray_marching/SceneManager.h"
#include "Panel.h"

namespace ui {
class ScenePanel : public Panel {
public:
  explicit ScenePanel(SceneManager &sceneManager);
  void onFrame() override;
  [[nodiscard]] auto isChoiceChanged() const -> bool;

  [[nodiscard]] auto getSelectedScene() -> std::shared_ptr<Scene> &;

private:
  SceneManager &sceneManager;
  char const *currentScene = "";
  std::shared_ptr<Scene> selectedScene = nullptr;
  std::vector<std::string> scenes;

  bool isChoiceChanged_ = true;

  void checkAndLoadScenes();
};
} // namespace ui

#endif // RAYMARCHING_SCENEPANEL_H
