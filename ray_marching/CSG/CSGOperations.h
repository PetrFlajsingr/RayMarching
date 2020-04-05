//
// Created by petr on 4/4/20.
//

#ifndef RAYMARCHING_CSGOPERATIONS_H
#define RAYMARCHING_CSGOPERATIONS_H

#include "CSGCommon.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Operation : public CSGRawData {
public:
  [[nodiscard]] virtual auto getName() const -> std::string = 0;
};

struct OperationUnion : public Operation {
  [[nodiscard]] auto getRaw() const -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() const -> std::size_t override;
  [[nodiscard]] auto src() const -> std::string override;

private:
  [[nodiscard]] auto getName() const -> std::string override;
};

struct OperationSubstraction : public Operation {
  [[nodiscard]] auto getRaw() const -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() const -> std::size_t override;
  [[nodiscard]] auto src() const -> std::string override;

private:
  [[nodiscard]] auto getName() const -> std::string override;
};

struct OperationBlend : public Operation {
  explicit OperationBlend(float k);
  [[nodiscard]] auto getRaw() const -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() const -> std::size_t override;
  [[nodiscard]] auto src() const -> std::string override;

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

#endif // RAYMARCHING_CSGOPERATIONS_H
