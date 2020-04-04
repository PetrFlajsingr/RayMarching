//
// Created by petr on 4/4/20.
//

#include "CSGTree.h"

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

ShapeCSGNode::ShapeCSGNode(std::unique_ptr<Shape> &&shape) : shape(std::move(shape)) {}
auto ShapeCSGNode::isLeaf() const -> bool { return true; }
auto ShapeCSGNode::getShape() -> Shape & { return *shape; }

auto OperationUnion::getRaw() -> std::vector<uint8_t> { return {operationFlag | flagForOperation<OperationUnion>()}; }
auto OperationUnion::getDataSize() -> std::size_t { return 1; }
std::string OperationUnion::getName() const { return "union"; }

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
