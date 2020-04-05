//
// Created by petr on 4/4/20.
//

#include "CSGOperations.h"
#include <fmt/format.h>

auto OperationUnion::getRaw() const -> std::vector<uint8_t> { return {operationFlag | flagForOperation<OperationUnion>()}; }
auto OperationUnion::getDataSize() const -> std::size_t { return 1; }
auto OperationUnion::getName() const -> std::string { return "union"; }
auto OperationUnion::src() const -> std::string { return "opUnion(\n{}, {})"; }
auto OperationUnion::eval(float d1, float d2) const -> float { return fncForOp<OperationUnion>()(d1, d2); }

auto OperationSubstraction::getRaw() const -> std::vector<uint8_t> {
  return {operationFlag | flagForOperation<OperationSubstraction>()};
}
auto OperationSubstraction::getDataSize() const -> std::size_t { return 1; }
auto OperationSubstraction::getName() const -> std::string { return "substraction"; }
auto OperationSubstraction::src() const -> std::string { return "opSubstraction(\n{}, {})"; }
auto OperationSubstraction::eval(float d1, float d2) const -> float { return fncForOp<OperationSubstraction>()(d1, d2); }

OperationBlend::OperationBlend(float k) : k(k) {}
auto OperationBlend::getRaw() const -> std::vector<uint8_t> {
  return {operationFlag | flagForOperation<OperationSubstraction>()};
}
auto OperationBlend::getDataSize() const -> std::size_t { return 1; }
auto OperationBlend::getName() const -> std::string { return "substraction"; }
auto OperationBlend::src() const -> std::string {
  using namespace std::string_literals;
  return "opBlend(\n{}, {}, "s + fmt::format("{}", k) + ")";
}
auto OperationBlend::eval(float d1, float d2) const -> float { return fncForOp<OperationBlend>()(d1, d2, k); }
