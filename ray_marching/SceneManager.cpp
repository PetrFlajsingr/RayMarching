//
// Created by petr on 5/1/20.
//

#include "SceneManager.h"
#include <spdlog/spdlog.h>

auto SceneManager::addScene(Scene &&scene) -> void {
  const auto sceneName = scene.getName();
  scenes[sceneName] = std::make_shared<Scene>(std::move(scene));
}
auto SceneManager::getScene(const std::string &name) -> std::shared_ptr<Scene> & { return scenes[name]; }
auto SceneManager::getScene(const std::string &name) const -> std::shared_ptr<const Scene> { return scenes[name]; }
auto SceneManager::loadFromJson(const nlohmann::json &json) -> void {
  using namespace std::string_literals;
  if (!json.contains("name") || !json.contains("objects")) {
    spdlog::error("Invalid scene: "s + json.dump(2));
    return;
  }
  Scene scene{json["name"], Camera{PerspectiveProjection{0, 0, 0, 0}}};
  for (const auto &item : json["objects"]) {
    auto object = Scene::SceneObject::FromJson(item);
    if (object.has_value()) {
      scene.addObject(object.value()->name, std::move(object.value()));
    }
  }
  addScene(std::move(scene));
}
