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

class OperationCSGNode;
class ShapeCSGNode;

class CSGNode {
public:
  [[nodiscard]] virtual auto isLeaf() const -> bool = 0;
  [[nodiscard]] virtual auto isBinary() const -> bool = 0;
  [[nodiscard]] virtual auto getRawData() -> std::vector<uint8_t> = 0;
  [[nodiscard]] virtual auto getRawDataSize() -> std::size_t = 0;
  virtual ~CSGNode() = default;
};

class WarpOperationNode : public CSGNode {
public:
  explicit WarpOperationNode(std::unique_ptr<SpaceWarpOperation> &&operation) : operation(std::move(operation)) {}

  template <C_Shape T, typename... Args> auto setChild(Args &&... args) -> ShapeCSGNode & {
    return reinterpret_cast<ShapeCSGNode &>(
        internalSetChild(std::make_unique<ShapeCSGNode>(std::make_unique<T>(std::forward<Args>(args)...))));
  }
  template <C_Operation T, typename... Args> auto setChild(Args &&... args) -> OperationCSGNode & {
    return reinterpret_cast<OperationCSGNode &>(
        internalSetChild(std::make_unique<OperationCSGNode>(std::make_unique<T>(std::forward<Args>(args)...))));
  }
  template <C_WarpOperation T, typename... Args> auto setChild(Args &&... args) -> WarpOperationNode & {
    return reinterpret_cast<WarpOperationNode &>(
        internalSetChild(std::make_unique<WarpOperationNode>(std::make_unique<T>(std::forward<Args>(args)...))));
  }

  [[nodiscard]] auto isLeaf() const -> bool override;
  [[nodiscard]] auto getRawData() -> std::vector<uint8_t> override;
  [[nodiscard]] auto getRawDataSize() -> std::size_t override;
  [[nodiscard]] auto eval(const glm::vec3 &camPos) -> glm::vec3;
  [[nodiscard]] auto getChild() -> CSGNode &;
  [[nodiscard]] auto isBinary() const -> bool override;
  [[nodiscard]] auto getOperation() -> SpaceWarpOperation &;

private:
  auto internalSetChild(std::unique_ptr<CSGNode> &&child) -> CSGNode &;
  std::unique_ptr<CSGNode> child;
  std::unique_ptr<SpaceWarpOperation> operation;
};

class ShapeCSGNode : public CSGNode {
public:
  explicit ShapeCSGNode(std::unique_ptr<Shape> &&shape);
  [[nodiscard]] auto isLeaf() const -> bool override;
  [[nodiscard]] auto getShape() -> Shape &;
  [[nodiscard]] auto getRawData() -> std::vector<uint8_t> override;
  [[nodiscard]] auto getRawDataSize() -> std::size_t override;
  [[nodiscard]] auto eval(const glm::vec3 &camPos) -> float;
  [[nodiscard]] auto isBinary() const -> bool override;

private:
  std::unique_ptr<Shape> shape;
};

class OperationCSGNode : public CSGNode {
public:
  explicit OperationCSGNode(std::unique_ptr<BinaryOperation> &&operation);

  template <C_Shape T, typename... Args> auto setLeftChild(Args &&... args) -> ShapeCSGNode & {
    return reinterpret_cast<ShapeCSGNode &>(
        internalSetLeftChild(std::make_unique<ShapeCSGNode>(std::make_unique<T>(std::forward<Args>(args)...))));
  }
  template <C_Operation T, typename... Args> auto setLeftChild(Args &&... args) -> OperationCSGNode & {
    return reinterpret_cast<OperationCSGNode &>(
        internalSetLeftChild(std::make_unique<OperationCSGNode>(std::make_unique<T>(std::forward<Args>(args)...))));
  }
  template <C_Shape T, typename... Args> auto setRightChild(Args &&... args) -> ShapeCSGNode & {
    return reinterpret_cast<ShapeCSGNode &>(
        internalSetRightChild(std::make_unique<ShapeCSGNode>(std::make_unique<T>(std::forward<Args>(args)...))));
  }
  template <C_Operation T, typename... Args> auto setRightChild(Args &&... args) -> OperationCSGNode & {
    return reinterpret_cast<OperationCSGNode &>(
        internalSetRightChild(std::make_unique<OperationCSGNode>(std::make_unique<T>(std::forward<Args>(args)...))));
  }

  template <C_WarpOperation T, typename... Args> auto setLeftChild(Args &&... args) -> WarpOperationNode & {
    return reinterpret_cast<WarpOperationNode &>(
        internalSetLeftChild(std::make_unique<WarpOperationNode>(std::make_unique<T>(std::forward<Args>(args)...))));
  }
  template <C_WarpOperation T, typename... Args> auto setRightChild(Args &&... args) -> WarpOperationNode & {
    return reinterpret_cast<WarpOperationNode &>(
        internalSetRightChild(std::make_unique<WarpOperationNode>(std::make_unique<T>(std::forward<Args>(args)...))));
  }

  [[nodiscard]] auto getRawData() -> std::vector<uint8_t> override;
  [[nodiscard]] auto getRawDataSize() -> std::size_t override;

  [[nodiscard]] auto getLeftChild() -> CSGNode &;
  [[nodiscard]] auto getRightChild() -> CSGNode &;
  [[nodiscard]] auto isLeaf() const -> bool override;
  [[nodiscard]] auto getOperation() -> BinaryOperation &;
  [[nodiscard]] auto eval(float d1, float d2) -> float;
  [[nodiscard]] auto isBinary() const -> bool override;

private:
  auto internalSetLeftChild(std::unique_ptr<CSGNode> &&leftChild) -> CSGNode &;
  auto internalSetRightChild(std::unique_ptr<CSGNode> &&rightChild) -> CSGNode &;

  std::unique_ptr<CSGNode> leftChild;
  std::unique_ptr<CSGNode> rightChild;
  std::unique_ptr<BinaryOperation> operation;
};
class CSGTree {
public:
  std::unique_ptr<CSGNode> root = nullptr;

  auto src() -> std::string;

  auto eval(const glm::vec3 &camPos) -> float;
  auto getNormal(const glm::vec3 &camPos) -> glm::vec3;
};

template <typename F> auto csgPreorder(CSGNode &node, F &&callable) -> void {
  if (node.isLeaf()) {
    callable(reinterpret_cast<ShapeCSGNode &>(node));
  } else if (node.isBinary()) {
    auto &opNode = reinterpret_cast<OperationCSGNode &>(node);
    csgPreorder(opNode.getLeftChild(), callable);
    callable(opNode);
    csgPreorder(opNode.getRightChild(), callable);
  } else {
    auto &opNode = reinterpret_cast<WarpOperationNode &>(node);
    csgPreorder(opNode.getChild(), callable);
    callable(opNode);
  }
}
template <typename F> auto csgInorder(CSGNode &node, F &&callable) -> void {
  if (node.isLeaf()) {
    callable(reinterpret_cast<ShapeCSGNode &>(node));
  } else if (node.isBinary()) {
    auto &opNode = reinterpret_cast<OperationCSGNode &>(node);
    callable(opNode);
    csgInorder(opNode.getLeftChild(), callable);
    csgInorder(opNode.getRightChild(), callable);
  } else {
    auto &opNode = reinterpret_cast<WarpOperationNode &>(node);
    callable(opNode);
    csgInorder(opNode.getChild(), callable);
  }
}

#endif // RAYMARCHING_CSGTREE_H
