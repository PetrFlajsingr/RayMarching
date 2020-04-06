//
// Created by petr on 4/4/20.
//

#ifndef RAYMARCHING_CSGOPERATIONS_H
#define RAYMARCHING_CSGOPERATIONS_H

#include "CSGCommon.h"
#include "CSGOperationsFunctions.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

class BinaryOperation : public CSGRawData {
public:
  [[nodiscard]] virtual auto getName() const -> std::string = 0;
  [[nodiscard]] virtual auto eval(float d1, float d2) const -> float = 0;
};

class SpaceWarpOperation : public CSGRawData {
public:
  [[nodiscard]] virtual auto getName() const -> std::string = 0;
  [[nodiscard]] virtual auto eval(const glm::vec3 &camPos) const -> glm::vec3 = 0;
};

class SpaceRepetitionOperation : public SpaceWarpOperation {
public:
  explicit SpaceRepetitionOperation(const glm::vec3 &domain);
  [[nodiscard]] auto getRaw() const -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() const -> std::size_t override;
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto getName() const -> std::string override;
  [[nodiscard]] auto eval(const glm::vec3 &camPos) const -> glm::vec3 override;

  glm::vec3 domain;
};

class LimitedSpaceRepetitionOperation : public SpaceWarpOperation {
public:
  LimitedSpaceRepetitionOperation(const glm::vec3 &domain, const glm::vec3 &limit);
  [[nodiscard]] auto getRaw() const -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() const -> std::size_t override;
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto getName() const -> std::string override;
  [[nodiscard]] auto eval(const glm::vec3 &camPos) const -> glm::vec3 override;

  glm::vec3 domain;
  glm::vec3 limit;
};

class SpaceStretchOperation : public SpaceWarpOperation {
public:
  explicit SpaceStretchOperation(const glm::vec3 &multiplier);
  [[nodiscard]] auto getRaw() const -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() const -> std::size_t override;
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto getName() const -> std::string override;
  [[nodiscard]] auto eval(const glm::vec3 &camPos) const -> glm::vec3 override;

  glm::vec3 multiplier;
};

struct OperationUnion : public BinaryOperation {
  [[nodiscard]] auto getRaw() const -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() const -> std::size_t override;
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto eval(float d1, float d2) const -> float override;
  [[nodiscard]] auto getName() const -> std::string override;
};

struct OperationSubstraction : public BinaryOperation {
  [[nodiscard]] auto getRaw() const -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() const -> std::size_t override;
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto eval(float d1, float d2) const -> float override;

private:
  [[nodiscard]] auto getName() const -> std::string override;
};

struct OperationBlend : public BinaryOperation {
  explicit OperationBlend(float k);
  [[nodiscard]] auto getRaw() const -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() const -> std::size_t override;
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto eval(float d1, float d2) const -> float override;
  float k;

private:
  [[nodiscard]] auto getName() const -> std::string override;
};

template <C_Operation T> constexpr uint8_t flagForOperation() {
  if constexpr (std::is_same_v<T, OperationUnion>) {
    return 0b00000000;
  } else if constexpr (std::is_same_v<T, OperationSubstraction>) {
    return flagForOperation<OperationUnion>() + 0b00000001;
  } else if constexpr (std::is_same_v<T, OperationBlend>) {
    return flagForOperation<OperationSubstraction>() + 0b00000001;
  }
  throw "invalid operation type";
}

template <C_Operation T> constexpr auto fncForOp() {
  if constexpr (std::is_same_v<T, OperationUnion>) {
    return csg_op::opUnion;
  } else if constexpr (std::is_same_v<T, OperationSubstraction>) {
    return csg_op::opMinus;
  } else if constexpr (std::is_same_v<T, OperationBlend>) {
    return csg_op::opBlend;
  }
}

#endif // RAYMARCHING_CSGOPERATIONS_H
