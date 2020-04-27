//
// Created by petr on 4/6/20.
//

#include "Scene.h"

Scene::Scene(Camera &&camera) : camera(std::move(camera)) {}
auto Scene::addObject(const Scene::ObjectId &id, std::unique_ptr<SceneObject> &&object) -> void {
  objects[id] = std::move(object);
}
auto Scene::getObject(const Scene::ObjectId &id) -> std::optional<std::reference_wrapper<SceneObject>> {
  if (!objects.contains(id)) {
    return std::nullopt;
  }
  return *objects[id];
}
auto Scene::getObject(const Scene::ObjectId &id) const -> std::optional<std::reference_wrapper<const SceneObject>> {
  if (!objects.contains(id)) {
    return std::nullopt;
  }
  return *objects[id];
}
auto Scene::getObjects() -> std::vector<std::reference_wrapper<SceneObject>> {
  std::vector<std::reference_wrapper<SceneObject>> result;
  for (auto &[id, object] : objects) {
    result.emplace_back(*object);
  }
  return result;
}
auto Scene::getObjects() const -> std::vector<std::reference_wrapper<const SceneObject>> {
  std::vector<std::reference_wrapper<const SceneObject>> result;
  for (const auto &[id, object] : objects) {
    result.emplace_back(*object);
  }
  return result;
}
auto Scene::getCamera() -> Camera & { return camera; }
auto Scene::getCamera() const -> const Camera & { return camera; }
auto Scene::getDistanceToScene(const glm::vec3 &pos) -> float {
  auto minDistance = std::numeric_limits<float>::max();
  for (auto &[id, object] : objects) {
    const auto distToObject = object->eval(pos);
    minDistance = std::min(minDistance, distToObject);
  }
  return minDistance;
}
auto Scene::getNormal(const glm::vec3 &pos) -> glm::vec3 {
  auto minDistance = std::numeric_limits<float>::max();
  ObjectId nearestId;
  for (auto &[id, object] : objects) {
    const auto distToObject = object->eval(pos);
    if (distToObject < minDistance) {
      nearestId = id;
      minDistance = distToObject;
    }
  }
  return getObject(nearestId)->get().getNormal(pos);
}
