//
// Created by petr on 4/11/20.
//

#ifndef RAYMARCHING_PHYSICSOBJECT_H
#define RAYMARCHING_PHYSICSOBJECT_H
#include <glm/vec3.hpp>
#include <utility>

class PhysicsObject {
public:
  PhysicsObject(const glm::vec3 &positionm, float mass);
  [[nodiscard]] auto getVelocity() const -> const glm::vec3 &;
  auto setVelocity(const glm::vec3 &velocity) -> void;
  [[nodiscard]] auto getPosition() const -> const glm::vec3 &;
  auto setPosition(const glm::vec3 &position) -> void;
  [[nodiscard]] auto getMass() const -> float;
  auto setMass(float mass) -> void;

private:
  glm::vec3 velocity{0};
  glm::vec3 position{0};
  float mass = 0;
};

#endif // RAYMARCHING_PHYSICSOBJECT_H
