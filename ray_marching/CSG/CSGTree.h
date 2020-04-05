//
// Created by petr on 4/4/20.
//

#ifndef RAYMARCHING_CSGTREE_H
#define RAYMARCHING_CSGTREE_H

#include "CSGOperations.h"
#include "CSGShapes.h"
#include <functional>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

class CSGNode {
public:
  [[nodiscard]] virtual auto isLeaf() const -> bool = 0;
  [[nodiscard]] virtual auto getRawData() -> std::vector<uint8_t> = 0;
  [[nodiscard]] virtual auto getRawDataSize() -> std::size_t = 0;
  virtual ~CSGNode() = default;
};

class ShapeCSGNode : public CSGNode {
public:
  explicit ShapeCSGNode(std::unique_ptr<Shape> &&shape);
  [[nodiscard]] auto isLeaf() const -> bool override;
  [[nodiscard]] auto getShape() -> Shape &;
  [[nodiscard]] auto getRawData() -> std::vector<uint8_t> override;
  [[nodiscard]] auto getRawDataSize() -> std::size_t override;
  [[nodiscard]] auto eval(const glm::vec3 &camPos) -> float;

private:
  std::unique_ptr<Shape> shape;
};

class OperationCSGNode : public CSGNode {
public:
  explicit OperationCSGNode(std::unique_ptr<Operation> &&operation);

  template <C_Shape T, typename... Args> auto setLeftChild(Args &&... args) -> CSGNode & {
    return internalSetLeftChild(std::make_unique<ShapeCSGNode>(std::make_unique<T>(std::forward<Args>(args)...)));
  }
  template <C_Operation T, typename... Args> auto setLeftChild(Args &&... args) -> CSGNode & {
    return internalSetLeftChild(std::make_unique<OperationCSGNode>(std::make_unique<T>(std::forward<Args>(args)...)));
  }
  template <C_Shape T, typename... Args> auto setRightChild(Args &&... args) -> CSGNode & {
    return internalSetRightChild(std::make_unique<ShapeCSGNode>(std::make_unique<T>(std::forward<Args>(args)...)));
  }
  template <C_Operation T, typename... Args> auto setRightChild(Args &&... args) -> CSGNode & {
    return internalSetRightChild(std::make_unique<OperationCSGNode>(std::make_unique<T>(std::forward<Args>(args)...)));
  }

  [[nodiscard]] auto getRawData() -> std::vector<uint8_t> override;
  [[nodiscard]] auto getRawDataSize() -> std::size_t override;

  [[nodiscard]] auto getLeftChild() -> CSGNode &;
  [[nodiscard]] auto getRightChild() -> CSGNode &;
  [[nodiscard]] auto isLeaf() const -> bool override;
  [[nodiscard]] auto getOperation() -> Operation &;
  [[nodiscard]] auto eval(float d1, float d2) -> float;

private:
  auto internalSetLeftChild(std::unique_ptr<CSGNode> &&leftChild) -> CSGNode &;
  auto internalSetRightChild(std::unique_ptr<CSGNode> &&rightChild) -> CSGNode &;

  std::unique_ptr<CSGNode> leftChild;
  std::unique_ptr<CSGNode> rightChild;
  std::unique_ptr<Operation> operation;
};
class CSGTree {
public:
  std::unique_ptr<CSGNode> root = nullptr;

  auto src() -> std::string;

  auto eval(const glm::vec3 &camPos) -> float;
  ;
};

template <typename F> auto csgPreorder(CSGNode &node, F &&callable) -> void {
  if (node.isLeaf()) {
    callable(reinterpret_cast<ShapeCSGNode &>(node));
  } else {
    auto &opNode = reinterpret_cast<OperationCSGNode &>(node);
    csgPreorder(opNode.getLeftChild(), callable);
    callable(opNode);
    csgPreorder(opNode.getRightChild(), callable);
  }
}
template <typename F> auto csgInorder(CSGNode &node, F &&callable) -> void {
  if (node.isLeaf()) {
    callable(reinterpret_cast<ShapeCSGNode &>(node));
  } else {
    auto &opNode = reinterpret_cast<OperationCSGNode &>(node);
    callable(opNode);
    csgInorder(opNode.getLeftChild(), callable);
    csgInorder(opNode.getRightChild(), callable);
  }
}



#endif // RAYMARCHING_CSGTREE_H
