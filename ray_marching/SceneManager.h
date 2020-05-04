//
// Created by petr on 5/1/20.
//

#ifndef RAYMARCHING_SCENEMANAGER_H
#define RAYMARCHING_SCENEMANAGER_H
#include "MaterialManager.h"
#include "Scene.h"
#include <nlohmann/json.hpp>
#include <unordered_map>

class SceneManager {
public:
  SceneManager() = default;
  auto loadFromJson(const nlohmann::json &json, const MaterialManager &materialManager) -> void;
  auto addScene(Scene &&scene) -> void;
  [[nodiscard]] auto getScene(const std::string &name) -> std::shared_ptr<Scene> &;
  [[nodiscard]] auto getScene(const std::string &name) const -> std::shared_ptr<const Scene>;

  [[nodiscard]] auto getScenes() -> std::unordered_map<std::string, std::shared_ptr<Scene>> &;

private:
  mutable std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
};

#endif // RAYMARCHING_SCENEMANAGER_H
