//
// Created by petr on 4/11/20.
//

#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(const glm::vec3 &position, float mass) : position(position), mass(mass) {}
auto PhysicsObject::getVelocity() const -> const glm::vec3 & { return velocity; }
auto PhysicsObject::setVelocity(const glm::vec3 &velocity) -> void { PhysicsObject::velocity = velocity; }
auto PhysicsObject::getPosition() const -> const glm::vec3 & { return position; }
auto PhysicsObject::setPosition(const glm::vec3 &position) -> void { PhysicsObject::position = position; }
auto PhysicsObject::getMass() const -> float { return mass; }
auto PhysicsObject::setMass(float mass) -> void { PhysicsObject::mass = mass; }
