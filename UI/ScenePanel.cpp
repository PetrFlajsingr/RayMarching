//
// Created by petr on 5/1/20.
//

#include "ScenePanel.h"
#include <ImGuiFileDialog/ImGuiFileDialog.h>
#include <fstream>
#include <imgui/imgui.h>

ui::ScenePanel::ScenePanel(SceneManager &sceneManager, const MaterialManager &materialManager)
    : sceneManager(sceneManager), materialManager(materialManager) {}

void ui::ScenePanel::onFrame() {
  checkAndLoadScenes();
  ImGui::Begin("Scenes");

  if (ImGui::BeginCombo("Active scene", currentScene)) {
    for (auto &choice : scenes) {
      bool isSelected = (currentScene == choice.data());
      if (ImGui::Selectable(choice.data(), isSelected)) {
        currentScene = choice.data();
        selectedScene = sceneManager.getScene(currentScene);
      }
      if (isSelected) {
        ImGui::SetItemDefaultFocus();
        isChoiceChanged_ = false;
      } else {
        isChoiceChanged_ = true;
      }
    }
    ImGui::EndCombo();
  }

  if (ImGui::Button("Load scene file")) {
    igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".json\0\0", ".");
  }
  if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChooseFileDlgKey")) {
    if (igfd::ImGuiFileDialog::Instance()->IsOk) {
      const auto filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
      nlohmann::json json;
      std::ifstream ifstream{filePathName};
      ifstream >> json;
      sceneManager.loadFromJson(json, materialManager);
    }
    igfd::ImGuiFileDialog::Instance()->CloseDialog("ChooseFileDlgKey");
  }

  ImGui::End();
}
void ui::ScenePanel::checkAndLoadScenes() {
  if (sceneManager.getScenes().size() != scenes.size()) {
    if (selectedScene == nullptr) {
      selectedScene = sceneManager.getScenes().begin()->second;
      currentScene = selectedScene->getName().data();
    }
    const std::string selected = currentScene;
    scenes.clear();
    for (auto &[name, scene] : sceneManager.getScenes()) {
      scenes.emplace_back(name);
    }
    for (auto &sceneName : scenes) {
      if (selected == sceneName) {
        currentScene = sceneName.data();
      }
    }
  }
}
auto ui::ScenePanel::isChoiceChanged() const -> bool { return isChoiceChanged_; }
auto ui::ScenePanel::getSelectedScene() -> std::shared_ptr<Scene> & { return selectedScene; }
