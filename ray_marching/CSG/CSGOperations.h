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
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto getName() const -> std::string override;
  [[nodiscard]] auto eval(const glm::vec3 &camPos) const -> glm::vec3 override;
  [[nodiscard]] auto rawTypeInfo() const -> uint32_t override;
  [[nodiscard]] auto rawParameters() const -> std::vector<float> override;

  glm::vec3 domain;
};

class LimitedSpaceRepetitionOperation : public SpaceWarpOperation {
public:
  LimitedSpaceRepetitionOperation(const glm::vec3 &domain, const glm::vec3 &limit);
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto getName() const -> std::string override;
  [[nodiscard]] auto eval(const glm::vec3 &camPos) const -> glm::vec3 override;

  [[nodiscard]] auto rawTypeInfo() const -> uint32_t override;
  [[nodiscard]] auto rawParameters() const -> std::vector<float> override;

  glm::vec3 domain;
  glm::vec3 limit;
};

class SpaceStretchOperation : public SpaceWarpOperation {
public:
  explicit SpaceStretchOperation(const glm::vec3 &multiplier);
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto getName() const -> std::string override;
  [[nodiscard]] auto eval(const glm::vec3 &camPos) const -> glm::vec3 override;

  [[nodiscard]] auto rawTypeInfo() const -> uint32_t override;
  [[nodiscard]] auto rawParameters() const -> std::vector<float> override;

  glm::vec3 multiplier;
};

struct OperationUnion : public BinaryOperation {
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto eval(float d1, float d2) const -> float override;
  [[nodiscard]] auto getName() const -> std::string override;

  [[nodiscard]] auto rawTypeInfo() const -> uint32_t override;
  [[nodiscard]] auto rawParameters() const -> std::vector<float> override;
};

struct OperationSubstraction : public BinaryOperation {
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto eval(float d1, float d2) const -> float override;
  [[nodiscard]] auto getName() const -> std::string override;

  [[nodiscard]] auto rawTypeInfo() const -> uint32_t override;
  [[nodiscard]] auto rawParameters() const -> std::vector<float> override;
};

struct OperationBlend : public BinaryOperation {
  explicit OperationBlend(float k);
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto eval(float d1, float d2) const -> float override;
  float k;
  [[nodiscard]] auto getName() const -> std::string override;

  [[nodiscard]] auto rawTypeInfo() const -> uint32_t override;
  [[nodiscard]] auto rawParameters() const -> std::vector<float> override;
};

struct OperationIntersection : public BinaryOperation {
  explicit OperationIntersection();
  [[nodiscard]] auto src() const -> std::string override;
  [[nodiscard]] auto eval(float d1, float d2) const -> float override;
  [[nodiscard]] auto getName() const -> std::string override;

  [[nodiscard]] auto rawTypeInfo() const -> uint32_t override;
  [[nodiscard]] auto rawParameters() const -> std::vector<float> override;
};

template <C_Operation T> constexpr auto flagForOperation() -> uint8_t {
  if constexpr (std::is_same_v<T, OperationUnion>) {
    return 0b0;
  } else if constexpr (std::is_same_v<T, OperationSubstraction>) {
    return flagForOperation<OperationUnion>() + 0b00000001;
  } else if constexpr (std::is_same_v<T, OperationBlend>) {
    return flagForOperation<OperationSubstraction>() + 0b00000001;
  } else if constexpr (std::is_same_v<T, OperationIntersection>) {
    return flagForOperation<OperationBlend>() + 0b00000001;
  }
}

template <C_WarpOperation T> constexpr auto flagForOperation() -> uint8_t {
  if constexpr (std::is_same_v<T, SpaceRepetitionOperation>) {
    return 0b0;
  } else if constexpr (std::is_same_v<T, LimitedSpaceRepetitionOperation>) {
    return flagForOperation<SpaceRepetitionOperation>() + 0b00000001;
  } else if constexpr (std::is_same_v<T, SpaceStretchOperation>) {
    return flagForOperation<LimitedSpaceRepetitionOperation>() + 0b00000001;
  }
}

template <C_Operation T> constexpr auto fncForOp() {
  if constexpr (std::is_same_v<T, OperationUnion>) {
    return csg_op::opUnion;
  } else if constexpr (std::is_same_v<T, OperationSubstraction>) {
    return csg_op::opMinus;
  } else if constexpr (std::is_same_v<T, OperationBlend>) {
    return csg_op::opBlend;
  } else if constexpr (std::is_same_v<T, OperationIntersection>) {
    return csg_op::opIntersection;
  }
}

#endif // RAYMARCHING_CSGOPERATIONS_H
