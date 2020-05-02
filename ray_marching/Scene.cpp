//
// Created by petr on 4/6/20.
//

#include "Scene.h"

Scene::Scene(std::string name, Camera &&camera)
    : name(std::move(name)), camera(std::move(camera)), ssboTree(nullptr), ssboParams(nullptr) {}
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
auto Scene::getName() const -> const std::string & { return name; }
Scene::Scene(Scene &&other) noexcept : name(std::move(other.name)), camera(std::move(other.camera)) {
  objects = std::move(other.objects);
  treeBuffer = std::move(other.treeBuffer);
  paramBuffer = std::move(other.paramBuffer);
}
auto Scene::operator=(Scene &&other) noexcept -> Scene & {
  camera = std::move(other.camera);
  name = std::move(other.name);
  objects = std::move(other.objects);
  treeBuffer = std::move(other.treeBuffer);
  paramBuffer = std::move(other.paramBuffer);
  return *this;
}
void Scene::updateAndBind(GLuint treeBindLocation, GLuint paramsBindLocation) {
  if (hasChanged) {
    auto setBufferData = [](auto &buffer, auto &data) {
      using DataType = typename std::decay_t<decltype(data)>::value_type;
      if (buffer == nullptr || static_cast<std::size_t>(buffer->getSize()) != data.size() * sizeof(DataType)) {
        buffer = std::make_shared<ge::gl::Buffer>(data.size() * sizeof(DataType), data.data());
      } else {
        buffer->setData(data);
      }
    };
    const auto rawTreeData = std::vector<uint8_t>{};
    const auto rawParamData = std::vector<float>{};
    setBufferData(ssboTree, rawTreeData);
    setBufferData(ssboParams, rawParamData);
    hasChanged = false;
  }
  ssboTree->bindBase(GL_SHADER_STORAGE_BUFFER, treeBindLocation);
  ssboParams->bindBase(GL_SHADER_STORAGE_BUFFER, paramsBindLocation);
}
auto Scene::raw() const -> CSGTree::Raw {
  CSGTree::Raw result;
  for (auto &[id, object] : objects) {
    const auto rawObject = object->raw(result.treeData.size() / 3, result.paramData.size());
    result.treeData.insert(result.treeData.end(), rawObject.treeData.begin(), rawObject.treeData.end());
    result.paramData.insert(result.paramData.end(), rawObject.paramData.begin(), rawObject.paramData.end());
  }
  return result;
}
