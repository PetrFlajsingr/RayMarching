//
// Created by petr on 4/11/20.
//

#ifndef RAYMARCHING_PHYSICSSIMULATOR_H
#define RAYMARCHING_PHYSICSSIMULATOR_H

#include "../ray_marching/Scene.h"
#include "PhysicsObject.h"
#include <list>
#include <memory>
#include <utility>

class PhysicsSimulator {
public:
  explicit PhysicsSimulator(std::shared_ptr<Scene> scene);
  auto addObject(std::shared_ptr<PhysicsObject> physicsObject) -> void;
  [[nodiscard]] auto getObjects() -> std::list<std::shared_ptr<PhysicsObject>> &;
  [[nodiscard]] auto getGravity() const -> const glm::vec3 &;
  auto setGravity(const glm::vec3 &gravity) -> void;

  auto update(float time) -> void;

private:
  float time = 0;
  std::shared_ptr<Scene> scene;
  std::list<std::shared_ptr<PhysicsObject>> physicsObjects;
  glm::vec3 gravity{0, -9.8, 0};
};

#endif // RAYMARCHING_PHYSICSSIMULATOR_H
