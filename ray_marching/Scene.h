//
// Created by petr on 4/6/20.
//

#ifndef RAYMARCHING_SCENE_H
#define RAYMARCHING_SCENE_H

#include "CSG/CSGTree.h"
#include <Camera.h>

class Scene {
public:
  using SceneObject = CSGTree;
  using ObjectId = std::string;

  explicit Scene(Camera &&camera);

  auto addObject(const ObjectId &id, std::unique_ptr<SceneObject> &&object) -> void;
  [[nodiscard]] auto getObject(const ObjectId &id) -> SceneObject &;
  [[nodiscard]] auto getObjects() -> std::vector<std::reference_wrapper<SceneObject>>;
  [[nodiscard]] auto getObjects() const -> std::vector<std::reference_wrapper<const SceneObject>>;
  [[nodiscard]] auto getCamera() -> Camera &;
  [[nodiscard]] auto getCamera() const -> const Camera &;
  [[nodiscard]] auto getDistanceToScene(const glm::vec3 &pos) -> float;
  [[nodiscard]] auto getNormal(const glm::vec3 &pos) -> glm::vec3;

private:
  std::unordered_map<ObjectId, std::unique_ptr<SceneObject>> objects;
  Camera camera;
};

#endif // RAYMARCHING_SCENE_H
