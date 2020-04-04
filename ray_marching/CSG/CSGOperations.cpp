//
// Created by petr on 4/4/20.
//

#include "CSGOperations.h"
#include <fmt/format.h>

auto OperationUnion::getRaw() -> std::vector<uint8_t> { return {operationFlag | flagForOperation<OperationUnion>()}; }
auto OperationUnion::getDataSize() -> std::size_t { return 1; }
auto OperationUnion::getName() const -> std::string { return "union"; }
auto OperationUnion::src() -> std::string { return "opUnion(\n{}, {})"; }

auto OperationSubstraction::getRaw() -> std::vector<uint8_t> {
  return {operationFlag | flagForOperation<OperationSubstraction>()};
}
auto OperationSubstraction::getDataSize() -> std::size_t { return 1; }
auto OperationSubstraction::getName() const -> std::string { return "substraction"; }
auto OperationSubstraction::src() -> std::string { return "opSubstraction(\n{}, {})"; }

OperationBlend::OperationBlend(float k) : k(k) {}
auto OperationBlend::getRaw() -> std::vector<uint8_t> { return {operationFlag | flagForOperation<OperationSubstraction>()}; }
auto OperationBlend::getDataSize() -> std::size_t { return 1; }
auto OperationBlend::getName() const -> std::string { return "substraction"; }
auto OperationBlend::src() -> std::string {
  using namespace std::string_literals;
  return "opBlend(\n{}, {}, "s + fmt::format("{}", k) + ")";
}