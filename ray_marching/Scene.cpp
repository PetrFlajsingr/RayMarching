//
// Created by petr on 4/6/20.
//

#include "Scene.h"
#include "types/Range.h"

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
void Scene::updateAndBind(GLuint treeBindLocation, GLuint paramsBindLocation, GLuint postOrderBindLocation) {
  if (hasChanged) {
    auto setBufferData = [](auto &buffer, auto &data) {
      using DataType = typename std::decay_t<decltype(data)>::value_type;
      if (buffer == nullptr || static_cast<std::size_t>(buffer->getSize()) != data.size() * sizeof(DataType)) {
        buffer = std::make_shared<ge::gl::Buffer>(data.size() * sizeof(DataType), data.data());
      } else {
        buffer->setData(data);
      }
    };
    const auto rawData = raw();
    setBufferData(ssboTree, rawData.treeData);
    setBufferData(ssboParams, rawData.paramData);
    setBufferData(ssboPostOrder, rawData.postOrderTraversal);
    hasChanged = false;
  }
  ssboTree->bindBase(GL_SHADER_STORAGE_BUFFER, treeBindLocation);
  ssboParams->bindBase(GL_SHADER_STORAGE_BUFFER, paramsBindLocation);
  ssboPostOrder->bindBase(GL_SHADER_STORAGE_BUFFER, postOrderBindLocation);
}
auto Scene::raw() const -> CSGTree::Raw {
  CSGTree::Raw result;
  result.treeData.resize((objects.size() - 1) * 3);
  result.postOrderTraversal.resize(1);
  result.postOrderTraversal[0] = 0;

  const auto objectCount = objects.size();
  int currentConnectNodeIndex = objectCount - 2;
  auto getConnectNodeRawData = [](uint32_t leftChildIndex, uint32_t rightChildIndex) -> std::array<uint32_t, 3> {
    return {flagForOperation<OperationUnion>() | rawCategory<OperationUnion>(), 0, rightChildIndex << 16u | leftChildIndex};
  };
  for (auto &[id, object] : objects) {
    const auto rightChildIndex = result.treeData.size() / 3;
    const auto rawObject = object->raw(result.treeData.size() / 3, result.paramData.size());
    const auto leftChildIndex = (result.treeData.size() + rawObject.treeData.size()) / 3;
    if (objectCount > 1 && currentConnectNodeIndex >= 0) {
      const auto connectionNodeRaw = getConnectNodeRawData(leftChildIndex, rightChildIndex);

      result.treeData[currentConnectNodeIndex * 3] = connectionNodeRaw[0];
      result.treeData[currentConnectNodeIndex * 3 + 1] = connectionNodeRaw[1];
      result.treeData[currentConnectNodeIndex * 3 + 2] = connectionNodeRaw[2];
    }

    result.treeData.insert(result.treeData.end(), rawObject.treeData.begin(), rawObject.treeData.end());
    result.paramData.insert(result.paramData.end(), rawObject.paramData.begin(), rawObject.paramData.end());
    result.postOrderTraversal[0] += rawObject.postOrderTraversal[0];
    result.postOrderTraversal.insert(result.postOrderTraversal.end(), rawObject.postOrderTraversal.begin() + 1,
                                     rawObject.postOrderTraversal.end());

    --currentConnectNodeIndex;
  }
  if (objectCount > 1) {
    auto additionalNodesRange = MakeRange::range<uint32_t>(0, objectCount - 2);
    result.postOrderTraversal[0] += objectCount - 1;
    result.postOrderTraversal.insert(result.postOrderTraversal.begin() + 1, additionalNodesRange.begin(),
                                     additionalNodesRange.end());
  }
  return result;
}
