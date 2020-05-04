//
// Created by petr on 4/4/20.
//

#ifndef RAYMARCHING_CSGTREE_H
#define RAYMARCHING_CSGTREE_H

#include "../MaterialManager.h"
#include "CSGOperations.h"
#include "CSGShapes.h"
#include <functional>
#include <glm/vec3.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <queue>
#include <stack>
#include <vector>

class OperationCSGNode;
class ShapeCSGNode;

enum class NodeType { WarpOperation, Operation, Shape };

class CSGNode {
public:
  [[nodiscard]] virtual auto isLeaf() const -> bool = 0;
  [[nodiscard]] virtual auto isBinary() const -> bool = 0;

  [[nodiscard]] virtual auto getCSGData() -> CSGRawData & = 0;
  [[nodiscard]] virtual auto getCSGData() const -> const CSGRawData & = 0;

  virtual ~CSGNode() = default;
};

class WarpOperationCSGNode : public CSGNode {
public:
  explicit WarpOperationCSGNode(std::unique_ptr<SpaceWarpOperation> &&operation) : operation(std::move(operation)) {}

  template <C_Shape T, typename... Args> auto setChild(Args &&... args) -> ShapeCSGNode & {
    return reinterpret_cast<ShapeCSGNode &>(
        internalSetChild(std::make_unique<ShapeCSGNode>(std::make_unique<T>(std::forward<Args>(args)...))));
  }
  template <C_Operation T, typename... Args> auto setChild(Args &&... args) -> OperationCSGNode & {
    return reinterpret_cast<OperationCSGNode &>(
        internalSetChild(std::make_unique<OperationCSGNode>(std::make_unique<T>(std::forward<Args>(args)...))));
  }
  template <C_WarpOperation T, typename... Args> auto setChild(Args &&... args) -> WarpOperationCSGNode & {
    return reinterpret_cast<WarpOperationCSGNode &>(
        internalSetChild(std::make_unique<WarpOperationCSGNode>(std::make_unique<T>(std::forward<Args>(args)...))));
  }

  auto setChild(std::unique_ptr<CSGNode> &&child) -> CSGNode &;

  [[nodiscard]] auto isLeaf() const -> bool override;
  [[nodiscard]] auto eval(const glm::vec3 &camPos) -> glm::vec3;
  [[nodiscard]] auto getChild() -> CSGNode &;
  [[nodiscard]] auto getChild() const -> const CSGNode &;
  [[nodiscard]] auto isBinary() const -> bool override;
  [[nodiscard]] auto getOperation() -> SpaceWarpOperation &;
  [[nodiscard]] auto getOperation() const -> const SpaceWarpOperation &;

  [[nodiscard]] auto getCSGData() -> CSGRawData & override;
  [[nodiscard]] auto getCSGData() const -> const CSGRawData & override;

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
  [[nodiscard]] auto getShape() const -> const Shape &;
  [[nodiscard]] auto eval(const glm::vec3 &camPos) -> float;
  [[nodiscard]] auto isBinary() const -> bool override;

  [[nodiscard]] auto getCSGData() -> CSGRawData & override;
  [[nodiscard]] auto getCSGData() const -> const CSGRawData & override;

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

  template <C_WarpOperation T, typename... Args> auto setLeftChild(Args &&... args) -> WarpOperationCSGNode & {
    return reinterpret_cast<WarpOperationCSGNode &>(
        internalSetLeftChild(std::make_unique<WarpOperationCSGNode>(std::make_unique<T>(std::forward<Args>(args)...))));
  }
  template <C_WarpOperation T, typename... Args> auto setRightChild(Args &&... args) -> WarpOperationCSGNode & {
    return reinterpret_cast<WarpOperationCSGNode &>(
        internalSetRightChild(std::make_unique<WarpOperationCSGNode>(std::make_unique<T>(std::forward<Args>(args)...))));
  }

  auto setLeftChild(std::unique_ptr<CSGNode> &&child) -> CSGNode &;
  auto setRightChild(std::unique_ptr<CSGNode> &&child) -> CSGNode &;

  [[nodiscard]] auto getLeftChild() -> CSGNode &;
  [[nodiscard]] auto getRightChild() -> CSGNode &;
  [[nodiscard]] auto getLeftChild() const -> const CSGNode &;
  [[nodiscard]] auto getRightChild() const -> const CSGNode &;
  [[nodiscard]] auto isLeaf() const -> bool override;
  [[nodiscard]] auto getOperation() -> BinaryOperation &;
  [[nodiscard]] auto getOperation() const -> const BinaryOperation &;
  [[nodiscard]] auto eval(float d1, float d2) -> float;
  [[nodiscard]] auto isBinary() const -> bool override;

  [[nodiscard]] auto getCSGData() -> CSGRawData & override;
  [[nodiscard]] auto getCSGData() const -> const CSGRawData & override;

private:
  auto internalSetLeftChild(std::unique_ptr<CSGNode> &&leftChild) -> CSGNode &;
  auto internalSetRightChild(std::unique_ptr<CSGNode> &&rightChild) -> CSGNode &;

  std::unique_ptr<CSGNode> leftChild;
  std::unique_ptr<CSGNode> rightChild;
  std::unique_ptr<BinaryOperation> operation;
};
class CSGTree {
public:
  struct Raw {
    /*
    GLSL code:
    struct CSGNode {
        uint categoryType;// fist 2 bits  - 0 operation, 1 warp operation, 2 shape, rest type of shape/operation
        uint paramOffset;
        uint childIndices;
    };
     */
    std::vector<uint32_t> treeData;
    std::vector<float> paramData;
    std::vector<uint32_t> postOrderTraversal;
  };

  explicit CSGTree(std::string name);
  [[nodiscard]] static auto FromJson(const nlohmann::json &json, const MaterialManager &materialManager)
      -> std::optional<std::unique_ptr<CSGTree>>;

  std::unique_ptr<CSGNode> root = nullptr;

  [[nodiscard]] auto src() -> std::string;
  [[nodiscard]] auto raw(std::size_t nodeOffset = 0, std::size_t paramOffset = 0) const -> Raw;

  [[nodiscard]] auto eval(const glm::vec3 &camPos) -> float;
  [[nodiscard]] auto getNormal(const glm::vec3 &camPos) -> glm::vec3;

  std::string name;

private:
  static auto NodeFromJson(const nlohmann::json &json, int materialIndex) -> std::optional<std::unique_ptr<CSGNode>>;
  static auto WarpNodeFromJson(const nlohmann::json &json, int materialIndex)
      -> std::optional<std::unique_ptr<WarpOperationCSGNode>>;
  static auto OperationNodeFromJson(const nlohmann::json &json, int materialIndex)
      -> std::optional<std::unique_ptr<OperationCSGNode>>;
  static auto ShapeNodeFromJson(const nlohmann::json &json, int materialIndex) -> std::optional<std::unique_ptr<ShapeCSGNode>>;
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
    auto &opNode = reinterpret_cast<WarpOperationCSGNode &>(node);
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
    auto &opNode = reinterpret_cast<WarpOperationCSGNode &>(node);
    callable(opNode);
    csgInorder(opNode.getChild(), callable);
  }
}
template <typename F> auto csgPostorder(CSGNode &node, F &&callable) -> void {
  if (node.isLeaf()) {
    callable(reinterpret_cast<ShapeCSGNode &>(node));
  } else if (node.isBinary()) {
    auto &opNode = reinterpret_cast<OperationCSGNode &>(node);
    csgInorder(opNode.getLeftChild(), callable);
    csgInorder(opNode.getRightChild(), callable);
    callable(opNode);
  } else {
    auto &opNode = reinterpret_cast<WarpOperationCSGNode &>(node);
    csgInorder(opNode.getChild(), callable);
    callable(opNode);
  }
}
template <typename F> auto csgBreadthFirst(CSGNode &node, F &&callable) -> void {
  std::queue<CSGNode *> queue;
  queue.emplace(&node);
  while (!queue.empty()) {
    auto &current = *queue.front();
    queue.pop();
    if (current.isLeaf()) {
      callable(reinterpret_cast<ShapeCSGNode &>(current));
    } else if (current.isBinary()) {
      auto &opNode = reinterpret_cast<OperationCSGNode &>(current);
      callable(opNode);
      queue.emplace(&opNode.getLeftChild());
      queue.emplace(&opNode.getRightChild());
    } else {
      auto &opNode = reinterpret_cast<WarpOperationCSGNode &>(current);
      callable(opNode);
      queue.emplace(&opNode.getChild());
    }
  }
}
template <typename F> auto csgDepthFirst(CSGNode &node, F &&callable) -> void {
  std::stack<CSGNode *> stack;
  stack.emplace(&node);
  while (!stack.empty()) {
    auto &current = *stack.top();
    stack.pop();
    callable(current);
    if (current.isLeaf()) {
      callable(reinterpret_cast<ShapeCSGNode &>(node));
    } else if (current.isBinary()) {
      auto &opNode = reinterpret_cast<OperationCSGNode &>(node);
      callable(opNode);
      stack.emplace(&opNode.getRightChild());
      stack.emplace(&opNode.getLeftChild());
    } else {
      auto &opNode = reinterpret_cast<WarpOperationCSGNode &>(node);
      callable(opNode);
      stack.emplace(&opNode.getChild());
    }
  }
}
#endif // RAYMARCHING_CSGTREE_H
