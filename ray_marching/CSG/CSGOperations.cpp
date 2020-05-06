//
// Created by petr on 4/4/20.
//

#include "CSGOperations.h"
#include <fmt/format.h>

auto OperationUnion::getName() const -> std::string { return "union"; }
auto OperationUnion::src() const -> std::string { return "opUnion(\n{}, {})\n"; }
auto OperationUnion::eval(float d1, float d2) const -> float { return fncForOp<OperationUnion>()(d1, d2); }
auto OperationUnion::rawTypeInfo() const -> uint32_t {
  return flagForOperation<std::decay_t<decltype(*this)>>() | rawCategory<std::decay_t<decltype(*this)>>();
}
auto OperationUnion::rawParameters() const -> std::vector<float> { return std::vector<float>{}; }

auto OperationSubstraction::getName() const -> std::string { return "substraction"; }
auto OperationSubstraction::src() const -> std::string { return "opSubstraction(\n{}, {})\n"; }
auto OperationSubstraction::eval(float d1, float d2) const -> float { return fncForOp<OperationSubstraction>()(d1, d2); }
auto OperationSubstraction::rawTypeInfo() const -> uint32_t {
  return flagForOperation<std::decay_t<decltype(*this)>>() | rawCategory<std::decay_t<decltype(*this)>>();
}
auto OperationSubstraction::rawParameters() const -> std::vector<float> { return std::vector<float>(); }

OperationBlend::OperationBlend(float k) : k(k) {}
auto OperationBlend::getName() const -> std::string { return "substraction"; }
auto OperationBlend::src() const -> std::string {
  using namespace std::string_literals;
  return "opBlend(\n{}, {}, "s + fmt::format("{}", k) + "\n)";
}
auto OperationBlend::eval(float d1, float d2) const -> float { return fncForOp<OperationBlend>()(d1, d2, k); }
auto OperationBlend::rawTypeInfo() const -> uint32_t {
  return flagForOperation<std::decay_t<decltype(*this)>>() | rawCategory<std::decay_t<decltype(*this)>>();
}
auto OperationBlend::rawParameters() const -> std::vector<float> { return std::vector<float>{k}; }

SpaceRepetitionOperation::SpaceRepetitionOperation(const glm::vec3 &domain) : domain(domain) {}
auto SpaceRepetitionOperation::src() const -> std::string {
  return fmt::format("camPos = opRep(camPos, vec3({}, {}, {}));\n", domain.x, domain.y, domain.z) + "{}";
}
auto SpaceRepetitionOperation::getName() const -> std::string { return "repetition"; }
auto SpaceRepetitionOperation::eval(const glm::vec3 &camPos) const -> glm::vec3 { return csg_op::opRep(camPos, domain); }
auto SpaceRepetitionOperation::rawTypeInfo() const -> uint32_t {
  return flagForOperation<std::decay_t<decltype(*this)>>() | rawCategory<std::decay_t<decltype(*this)>>();
}
auto SpaceRepetitionOperation::rawParameters() const -> std::vector<float> {
  return std::vector<float>{domain.x, domain.y, domain.z};
}

SpaceStretchOperation::SpaceStretchOperation(const glm::vec3 &multiplier) : multiplier(multiplier) {}
auto SpaceStretchOperation::src() const -> std::string {
  return fmt::format("camPos = opStretch(camPos, vec3({}, {}, {}));\n", multiplier.x, multiplier.y, multiplier.z) + "{}";
}
auto SpaceStretchOperation::getName() const -> std::string { return "stretch"; }
auto SpaceStretchOperation::eval(const glm::vec3 &camPos) const -> glm::vec3 { return csg_op::opStretch(camPos, multiplier); }
auto SpaceStretchOperation::rawTypeInfo() const -> uint32_t {
  return flagForOperation<std::decay_t<decltype(*this)>>() | rawCategory<std::decay_t<decltype(*this)>>();
}
auto SpaceStretchOperation::rawParameters() const -> std::vector<float> {
  return std::vector<float>{multiplier.x, multiplier.y, multiplier.z};
}

LimitedSpaceRepetitionOperation::LimitedSpaceRepetitionOperation(const glm::vec3 &domain, const glm::vec3 &limit)
    : domain(domain), limit(limit) {}
auto LimitedSpaceRepetitionOperation::src() const -> std::string {
  return fmt::format("camPos = opRepLim(camPos, vec3({}, {}, {}), vec3({}, {}, {}));\n", domain.x, domain.y, domain.z, limit.x,
                     limit.y, limit.z) +
         "{}";
}
auto LimitedSpaceRepetitionOperation::getName() const -> std::string { return "limited repetition"; }
auto LimitedSpaceRepetitionOperation::eval(const glm::vec3 &camPos) const -> glm::vec3 {
  return csg_op::opRepLim(camPos, domain, limit);
}
auto LimitedSpaceRepetitionOperation::rawTypeInfo() const -> uint32_t {
  return flagForOperation<std::decay_t<decltype(*this)>>() | rawCategory<std::decay_t<decltype(*this)>>();
}
auto LimitedSpaceRepetitionOperation::rawParameters() const -> std::vector<float> {
  return std::vector<float>{domain.x, domain.y, domain.z, limit.x, limit.y, limit.z};
}
OperationIntersection::OperationIntersection() {}
auto OperationIntersection::src() const -> std::string {
  using namespace std::string_literals;
  return "opIntersection(\n{}, {})";
}
auto OperationIntersection::eval(float d1, float d2) const -> float { return fncForOp<OperationIntersection>()(d1, d2); }
auto OperationIntersection::getName() const -> std::string { return "intersection"; }
auto OperationIntersection::rawTypeInfo() const -> uint32_t {
  return flagForOperation<std::decay_t<decltype(*this)>>() | rawCategory<std::decay_t<decltype(*this)>>();
}
auto OperationIntersection::rawParameters() const -> std::vector<float> { return {}; }
