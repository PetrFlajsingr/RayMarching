//
// Created by petr on 4/11/20.
//

#include "PhysicsSimulator.h"
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

PhysicsSimulator::PhysicsSimulator(std::shared_ptr<Scene> scene) : scene(std::move(scene)) {}

auto PhysicsSimulator::addObject(std::shared_ptr<PhysicsObject> physicsObject) -> void {
  physicsObjects.emplace_back(std::move(physicsObject));
}

auto PhysicsSimulator::update(float time) -> void {
  const float drag = .5;
  const float bounceCoef = 0.2;
  const auto deltaTime = time - PhysicsSimulator::time;
  if (deltaTime == 0) {
    return;
  }
  std::vector<std::function<void()>> finals;
  for (auto &objPtr : physicsObjects) {
    auto &obj = *objPtr;
    const auto mass = obj.getMass();
    auto position = obj.getPosition();
    auto velocity = obj.getVelocity();
    velocity += gravity * mass * deltaTime;
    obj.setVelocity(velocity);
    obj.setPosition(position + velocity * deltaTime);

    const auto distanceToScene = tree->eval(obj.getPosition()) - 10;
    if (distanceToScene < 0) {
      const auto distanceToSurface = distanceToScene;
      const auto positionDifference = obj.getPosition() - position;
      const auto normal = tree->getNormal(position + (distanceToSurface)*positionDifference);
      const auto dist = glm::distance(obj.getPosition(), position);
      glm::vec3 newPosition;
      glm::vec3 newVelocity;
      if (glm::length(obj.getVelocity()) < 1000 && glm::length(obj.getVelocity()) > 0.1) {
        const auto delta = normal * glm::dot(position - obj.getPosition(), normal);
        newPosition = obj.getPosition() + delta;
        newPosition += normal * 0.00001f;
        newVelocity = glm::reflect(obj.getVelocity(), normal);
        newVelocity = newVelocity * drag;
      } else {
        newPosition = position + normal * dist;
        newVelocity = glm::reflect(obj.getVelocity(), normal) * bounceCoef;
      }
      obj.setVelocity(newVelocity);
      obj.setPosition(newPosition);
    }

    // position = obj->getPosition();
    // velocity = obj->getVelocity();
    // for (auto &obj2 : physicsObjects) {
    //  if (obj == obj2) {
    //    continue;
    //  }
    //  const auto distanceToObj = glm::distance(obj->getPosition(), obj2->getPosition()) - 10;
    //  if (distanceToObj < 0) {
    //    const auto distanceToSurface = distanceToObj;
    //    const auto positionDifference = obj->getPosition() - position;
    //    const auto normal = glm::normalize(obj2->getPosition() - obj->getPosition());
    //    const auto dist = glm::distance(obj->getPosition(), position);
    //    const auto newPosition = position + normal * dist;
    //    finals.emplace_back([newPosition, normal, bounceCoef, obj, obj2] {
    //      const auto origVel = obj->getVelocity();
    //      obj->setVelocity(glm::reflect(obj->getVelocity(), normal) * bounceCoef);
    //      obj2->setVelocity(obj2->getVelocity() + (1 - bounceCoef) * origVel);
    //      obj->setPosition(newPosition);
    //    });
    //  }
    //}
  }
  std::for_each(finals.begin(), finals.end(), [](auto &f) { f(); });

  PhysicsSimulator::time = time;
}
auto PhysicsSimulator::getObjects() -> std::list<std::shared_ptr<PhysicsObject>> & { return physicsObjects; }

auto PhysicsSimulator::getGravity() const -> const glm::vec3 & { return gravity; }
auto PhysicsSimulator::setGravity(const glm::vec3 &gravity) -> void { PhysicsSimulator::gravity = gravity; }
