//
// Created by petr on 4/4/20.
//

#include "CSGTree.h"
#include <fmt/format.h>
#include <various/overload.h>

std::string replace(const std::string &str, const std::string &find, const std::string &replace) {
  std::string result;
  std::size_t find_len = find.size();
  std::size_t pos, from = 0;
  if (std::string::npos != (pos = str.find(find, from))) {
    result.append(str, from, pos - from);
    result.append(replace);
    from = pos + find_len;
  }
  result.append(str, from, std::string::npos);
  return result;
}

OperationCSGNode::OperationCSGNode(std::unique_ptr<Operation> &&operation) : operation(std::move(operation)) {}
auto OperationCSGNode::getLeftChild() -> CSGNode & { return *leftChild; }
auto OperationCSGNode::getRightChild() -> CSGNode & { return *rightChild; }
auto OperationCSGNode::isLeaf() const -> bool { return false; }
auto OperationCSGNode::getOperation() -> Operation & { return *operation; }
auto OperationCSGNode::internalSetLeftChild(std::unique_ptr<CSGNode> &&leftChild) -> CSGNode & {
  OperationCSGNode::leftChild = std::move(leftChild);
  return *OperationCSGNode::leftChild;
}
auto OperationCSGNode::internalSetRightChild(std::unique_ptr<CSGNode> &&rightChild) -> CSGNode & {
  OperationCSGNode::rightChild = std::move(rightChild);
  return *OperationCSGNode::rightChild;
}
auto OperationCSGNode::getRawData() -> std::vector<uint8_t> { return operation->getRaw(); }
auto OperationCSGNode::getRawDataSize() -> std::size_t { return operation->getDataSize(); }

ShapeCSGNode::ShapeCSGNode(std::unique_ptr<Shape> &&shape) : shape(std::move(shape)) {}
auto ShapeCSGNode::isLeaf() const -> bool { return true; }
auto ShapeCSGNode::getShape() -> Shape & { return *shape; }
auto ShapeCSGNode::getRawData() -> std::vector<uint8_t> { return shape->getRaw(); }
auto ShapeCSGNode::getRawDataSize() -> std::size_t { return shape->getDataSize(); }

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

auto BoxShape::getRaw() -> std::vector<uint8_t> {
  const auto positionRaw = vec3ToBytes(position);
  const auto dimensionsRaw = vec3ToBytes(dimensions);
  std::vector<uint8_t> result = {shapeFlag | flagForShape<BoxShape>()};
  for (auto byte : positionRaw) {
    result.emplace_back(byte);
  }
  for (auto byte : dimensionsRaw) {
    result.emplace_back(byte);
  }
  return result;
}
auto BoxShape::getDataSize() -> std::size_t { return 25; }
auto BoxShape::getName() const -> std::string { return "box"; }
auto BoxShape::src() -> std::string {
  const auto positionStr = fmt::format("vec3({}, {}, {})", position.x, position.y, position.z);
  const auto dimensionsStr = fmt::format("vec3({}, {}, {})", dimensions.x, dimensions.y, dimensions.z);
  return fmt::format("vec2(sdBox(camPos - {}, {}))", positionStr, dimensionsStr);
}

auto SphereShape::getRaw() -> std::vector<uint8_t> {
  const auto positionRaw = vec3ToBytes(position);
  const auto radiusRaw = floatToBytes(radius);
  std::vector<uint8_t> result = {shapeFlag | flagForShape<BoxShape>()};
  for (auto byte : positionRaw) {
    result.emplace_back(byte);
  }
  for (auto byte : radiusRaw) {
    result.emplace_back(byte);
  }
  return result;
}
auto SphereShape::getDataSize() -> std::size_t { return 17; }
auto SphereShape::getName() const -> std::string { return "sphere"; }
auto SphereShape::src() -> std::string {
  const auto positionStr = fmt::format("vec3({}, {}, {})", position.x, position.y, position.z);
  return fmt::format("vec2(sdSphere(camPos - {}, {}))", positionStr, radius);
}

auto CSGTree::src() -> std::string {
  std::string result = "{}";
  csgInorder(*root, overload{[&result](OperationCSGNode &node) { result = replace(result, "{}", node.getOperation().src()); },
                             [&result](ShapeCSGNode &node) { result = replace(result, "{}", node.getShape().src()); }});
  return result;
}
