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

  Scene(std::string name, Camera &&camera);
  Scene(Scene &&other) noexcept;
  auto operator=(Scene &&other) noexcept -> Scene &;

  auto addObject(const ObjectId &id, std::unique_ptr<SceneObject> &&object) -> void;
  [[nodiscard]] auto getObject(const ObjectId &id) -> std::optional<std::reference_wrapper<SceneObject>>;
  [[nodiscard]] auto getObject(const ObjectId &id) const -> std::optional<std::reference_wrapper<const SceneObject>>;
  [[nodiscard]] auto getObjects() -> std::vector<std::reference_wrapper<SceneObject>>;
  [[nodiscard]] auto getObjects() const -> std::vector<std::reference_wrapper<const SceneObject>>;
  [[nodiscard]] auto getCamera() -> Camera &;
  [[nodiscard]] auto getCamera() const -> const Camera &;
  [[nodiscard]] auto getDistanceToScene(const glm::vec3 &pos) -> float;
  [[nodiscard]] auto getNormal(const glm::vec3 &pos) -> glm::vec3;
  [[nodiscard]] auto getName() const -> const std::string &;

  [[nodiscard]] auto raw() const -> CSGTree::Raw;

  void updateAndBind(GLuint treeBindLocation, GLuint paramsBindLocation, GLuint postOrderBindLocation);

private:
  mutable std::unordered_map<ObjectId, std::unique_ptr<SceneObject>> objects;
  std::string name;
  Camera camera;

  bool hasChanged = true;

  std::shared_ptr<ge::gl::Buffer> ssboTree;
  std::shared_ptr<ge::gl::Buffer> ssboParams;
  std::shared_ptr<ge::gl::Buffer> ssboPostOrder;

  std::shared_ptr<ge::gl::Buffer> treeBuffer;
  std::shared_ptr<ge::gl::Buffer> paramBuffer;
};

#endif // RAYMARCHING_SCENE_H
