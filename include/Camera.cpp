//
// Created by petr on 10/19/19.
//

#include "Camera.h"

PerspectiveProjection::PerspectiveProjection(float near, float far, float aspectRatio, float fieldOfView)
    : near(near), far(far), aspectRatio(aspectRatio), fieldOfView(fieldOfView) {}

PerspectiveProjection::PerspectiveProjection(PerspectiveProjection &&other) noexcept {
  near = other.near;
  far = other.far;
  aspectRatio = other.aspectRatio;
  fieldOfView = other.fieldOfView;
  shouldUpdateMatrix = true;
}

float PerspectiveProjection::getAspectRatio() const { return aspectRatio; }
void PerspectiveProjection::setAspectRatio(float aspectRatio) {
  shouldUpdateMatrix = true;
  PerspectiveProjection::aspectRatio = aspectRatio;
}

float PerspectiveProjection::getFieldOfView() const { return fieldOfView; }
void PerspectiveProjection::setFieldOfView(float fieldOfView) {
  shouldUpdateMatrix = true;
  PerspectiveProjection::fieldOfView = fieldOfView;
}

float PerspectiveProjection::getNear() const { return near; }
void PerspectiveProjection::setNear(float near) {
  shouldUpdateMatrix = true;
  PerspectiveProjection::near = near;
}

float PerspectiveProjection::getFar() const { return far; }
void PerspectiveProjection::setFar(float far) {
  shouldUpdateMatrix = true;
  PerspectiveProjection::far = far;
}
