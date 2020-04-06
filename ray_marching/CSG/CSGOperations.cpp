//
// Created by petr on 4/4/20.
//

#include "CSGOperations.h"
#include <fmt/format.h>

auto OperationUnion::getRaw() const -> std::vector<uint8_t> { return {operationFlag | flagForOperation<OperationUnion>()}; }
auto OperationUnion::getDataSize() const -> std::size_t { return 1; }
auto OperationUnion::getName() const -> std::string { return "union"; }
auto OperationUnion::src() const -> std::string { return "opUnion(\n{}, {})\n"; }
auto OperationUnion::eval(float d1, float d2) const -> float { return fncForOp<OperationUnion>()(d1, d2); }

auto OperationSubstraction::getRaw() const -> std::vector<uint8_t> {
  return {operationFlag | flagForOperation<OperationSubstraction>()};
}
auto OperationSubstraction::getDataSize() const -> std::size_t { return 1; }
auto OperationSubstraction::getName() const -> std::string { return "substraction"; }
auto OperationSubstraction::src() const -> std::string { return "opSubstraction(\n{}, {})\n"; }
auto OperationSubstraction::eval(float d1, float d2) const -> float { return fncForOp<OperationSubstraction>()(d1, d2); }

OperationBlend::OperationBlend(float k) : k(k) {}
auto OperationBlend::getRaw() const -> std::vector<uint8_t> {
  return {operationFlag | flagForOperation<OperationSubstraction>()};
}
auto OperationBlend::getDataSize() const -> std::size_t { return 1; }
auto OperationBlend::getName() const -> std::string { return "substraction"; }
auto OperationBlend::src() const -> std::string {
  using namespace std::string_literals;
  return "opBlend(\n{}, {}, "s + fmt::format("{}", k) + "\n)";
}
auto OperationBlend::eval(float d1, float d2) const -> float { return fncForOp<OperationBlend>()(d1, d2, k); }

SpaceRepetitionOperation::SpaceRepetitionOperation(const glm::vec3 &domain) : domain(domain) {}
auto SpaceRepetitionOperation::getRaw() const -> std::vector<uint8_t> { return std::vector<uint8_t>(); }
auto SpaceRepetitionOperation::getDataSize() const -> std::size_t { return 0; }
auto SpaceRepetitionOperation::src() const -> std::string {
  return fmt::format("camPos = opRep(camPos, vec3({}, {}, {}));\n", domain.x, domain.y, domain.z) + "{}";
}
auto SpaceRepetitionOperation::getName() const -> std::string { return "repetition"; }
auto SpaceRepetitionOperation::eval(const glm::vec3 &camPos) const -> glm::vec3 { return csg_op::opRep(camPos, domain); }

SpaceStretchOperation::SpaceStretchOperation(const glm::vec3 &multiplier) : multiplier(multiplier) {}
auto SpaceStretchOperation::getRaw() const -> std::vector<uint8_t> { return std::vector<uint8_t>(); }
auto SpaceStretchOperation::getDataSize() const -> std::size_t { return 0; }
auto SpaceStretchOperation::src() const -> std::string {
  return fmt::format("camPos = opStretch(camPos, vec3({}, {}, {}));\n", multiplier.x, multiplier.y, multiplier.z) + "{}";
}
auto SpaceStretchOperation::getName() const -> std::string { return "stretch"; }
auto SpaceStretchOperation::eval(const glm::vec3 &camPos) const -> glm::vec3 { return csg_op::opStretch(camPos, multiplier); }

LimitedSpaceRepetitionOperation::LimitedSpaceRepetitionOperation(const glm::vec3 &domain, const glm::vec3 &limit)
    : domain(domain), limit(limit) {}
auto LimitedSpaceRepetitionOperation::getRaw() const -> std::vector<uint8_t> { return std::vector<uint8_t>(); }
auto LimitedSpaceRepetitionOperation::getDataSize() const -> std::size_t { return 0; }
auto LimitedSpaceRepetitionOperation::src() const -> std::string {
  return fmt::format("camPos = opRepLim(camPos, vec3({}, {}, {}), vec3({}, {}, {}));\n", domain.x, domain.y, domain.z, limit.x,
                     limit.y, limit.z) +
         "{}";
}
auto LimitedSpaceRepetitionOperation::getName() const -> std::string { return "limited repetition"; }
auto LimitedSpaceRepetitionOperation::eval(const glm::vec3 &camPos) const -> glm::vec3 {
  return csg_op::opRepLim(camPos, domain, limit);
}
