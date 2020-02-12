//
// Created by Petr on 19.11.2019.
//

template <unsigned int Dimensions> float geo::BoundingSphere<Dimensions>::distance(BoundingSphere::Point point) const {
  return glm::distance(center, point) - radius;
}
template <unsigned int Dimensions> geo::BoundingSphere<Dimensions>::BoundingSphere(Point center, float radius)
    : center(center), radius(radius) {}
