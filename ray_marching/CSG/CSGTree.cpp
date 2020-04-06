//
// Created by petr on 4/4/20.
//

#include "CSGTree.h"
#include <fmt/format.h>
#include <iostream>
#include <various/overload.h>

OperationCSGNode::OperationCSGNode(std::unique_ptr<BinaryOperation> &&operation) : operation(std::move(operation)) {}
auto OperationCSGNode::getLeftChild() -> CSGNode & { return *leftChild; }
auto OperationCSGNode::getRightChild() -> CSGNode & { return *rightChild; }
auto OperationCSGNode::isLeaf() const -> bool { return false; }
auto OperationCSGNode::getOperation() -> BinaryOperation & { return *operation; }
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
auto OperationCSGNode::eval(float d1, float d2) -> float { return operation->eval(d1, d2); }
auto OperationCSGNode::isBinary() const -> bool { return true; }

ShapeCSGNode::ShapeCSGNode(std::unique_ptr<Shape> &&shape) : shape(std::move(shape)) {}
auto ShapeCSGNode::isLeaf() const -> bool { return true; }
auto ShapeCSGNode::getShape() -> Shape & { return *shape; }
auto ShapeCSGNode::getRawData() -> std::vector<uint8_t> { return shape->getRaw(); }
auto ShapeCSGNode::getRawDataSize() -> std::size_t { return shape->getDataSize(); }
auto ShapeCSGNode::eval(const glm::vec3 &camPos) -> float { return shape->distance(camPos); }
auto ShapeCSGNode::isBinary() const -> bool { return false; }

auto CSGTree::src() -> std::string {
  std::string result = "{}";
  csgInorder(*root, overload{[&result](OperationCSGNode &node) { result = replace(result, "{}", node.getOperation().src()); },
                             [&result](WarpOperationNode &node) { result = replace(result, "{}", node.getOperation().src()); },
                             [&result](ShapeCSGNode &node) { result = replace(result, "{}", node.getShape().src()); }});
  return result;
}

auto evalSubtree(CSGNode &node, const glm::vec3 &camPos) -> float {
  if (node.isLeaf()) {
    auto &shapeNode = reinterpret_cast<ShapeCSGNode &>(node);
    return shapeNode.eval(camPos);
  } else if (node.isBinary()) {
    auto &operationNode = reinterpret_cast<OperationCSGNode &>(node);
    const auto left = evalSubtree(operationNode.getRightChild(), camPos);
    const auto right = evalSubtree(operationNode.getLeftChild(), camPos);
    return operationNode.eval(left, right);
  } else {
    auto &warpOperationNode = reinterpret_cast<WarpOperationNode &>(node);
    const auto warpedCamPos = warpOperationNode.eval(camPos);
    return evalSubtree(warpOperationNode.getChild(), warpedCamPos);
  }
}

auto CSGTree::eval(const glm::vec3 &camPos) -> float { return evalSubtree(*root, camPos); }
auto CSGTree::getNormal(const glm::vec3 &camPos) -> glm::vec3 {
  const glm::vec2 delta{0.0001f, 0.0f};
  return glm::normalize(glm::vec3(eval(camPos + delta.xyy()) - eval(camPos - delta.xyy()),
                                  eval(camPos + delta.yxy()) - eval(camPos - delta.yxy()),
                                  eval(camPos + delta.yyx()) - eval(camPos - delta.yyx())));
}
auto WarpOperationNode::internalSetChild(std::unique_ptr<CSGNode> &&child) -> CSGNode & {
  WarpOperationNode::child = std::move(child);
  return *WarpOperationNode::child;
}
auto WarpOperationNode::isLeaf() const -> bool { return false; }
auto WarpOperationNode::getRawData() -> std::vector<uint8_t> { return std::vector<uint8_t>(); }
auto WarpOperationNode::getRawDataSize() -> std::size_t { return 0; }
auto WarpOperationNode::eval(const glm::vec3 &camPos) -> glm::vec3 { return operation->eval(camPos); }
auto WarpOperationNode::getChild() -> CSGNode & { return *child; }
auto WarpOperationNode::isBinary() const -> bool { return false; }
auto WarpOperationNode::getOperation() -> SpaceWarpOperation & { return *operation; }
