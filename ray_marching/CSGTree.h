//
// Created by petr on 4/4/20.
//

#ifndef RAYMARCHING_CSGTREE_H
#define RAYMARCHING_CSGTREE_H

#include <glm/vec3.hpp>
#include <memory>
#include <vector>

inline auto vec3ToBytes(const glm::vec3 &vec) -> std::array<uint8_t, 12> {
  auto raw = reinterpret_cast<uint8_t const *>(&vec);
  std::array<uint8_t, 12> result;
  for (uint8_t i = 0; i < 12; ++i) {
    result[i] = raw[i];
  }
  return result;
}

inline auto floatToBytes(float val) -> std::array<uint8_t, 4> {
  auto raw = reinterpret_cast<uint8_t const *>(&val);
  std::array<uint8_t, 4> result;
  for (uint8_t i = 0; i < 4; ++i) {
    result[i] = raw[i];
  }
  return result;
}

constexpr uint8_t operationFlag = 0b00000000;
constexpr uint8_t shapeFlag = 0b10000000;

struct OperationUnion;
struct BoxShape;
struct SphereShape;
class Operation;
struct Shape;

template <typename T> concept C_Shape = std::is_base_of_v<Shape, T>;
template <typename T> concept C_Operation = std::is_base_of_v<Operation, T>;

template <typename T> constexpr uint8_t flagForOperation() {
  if constexpr (std::is_same_v<T, OperationUnion>) {
    return 0b00000000;
  }
  throw "invalid operation type";
}

template <typename T> constexpr uint8_t flagForShape() {
  if constexpr (std::is_same_v<T, BoxShape>) {
    return 0b00000000;
  }
  if constexpr (std::is_same_v<T, SphereShape>) {
    return flagForShape<BoxShape>() + 0b00000001;
  }
  throw "invalid operation type";
}

struct CSGRawData {
  [[nodiscard]] virtual auto getRaw() -> std::vector<uint8_t> = 0;
  [[nodiscard]] virtual auto getDataSize() -> std::size_t = 0;
  virtual ~CSGRawData() = default;
};

class Operation : public CSGRawData {
public:
  [[nodiscard]] virtual auto getName() const -> std::string = 0;
};

struct OperationUnion : public Operation {
  std::vector<uint8_t> getRaw() override;
  [[nodiscard]] auto getDataSize() -> std::size_t override;

private:
  [[nodiscard]] auto getName() const -> std::string override;
};

struct Shape : public CSGRawData {
  explicit Shape(const glm::vec3 &position) : position(position) {}
  glm::vec3 position;
  [[nodiscard]] virtual std::string getName() const = 0;
};

struct BoxShape : public Shape {
  BoxShape(const glm::vec3 &position, const glm::vec3 &dimensions) : Shape(position), dimensions(dimensions) {}
  glm::vec3 dimensions;
  [[nodiscard]] auto getRaw() -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() -> std::size_t override;
  [[nodiscard]] auto getName() const -> std::string override;
};

struct SphereShape : public Shape {
  SphereShape(const glm::vec3 &position, float radius) : Shape(position), radius(radius) {}
  float radius;
  [[nodiscard]] auto getRaw() -> std::vector<uint8_t> override;
  [[nodiscard]] auto getDataSize() -> std::size_t override;
  [[nodiscard]] auto getName() const -> std::string override;
};

class CSGNode {
public:
  [[nodiscard]] virtual auto isLeaf() const -> bool = 0;

  virtual ~CSGNode() = default;
};

class ShapeCSGNode : public CSGNode {
public:
  explicit ShapeCSGNode(std::unique_ptr<Shape> &&shape);
  [[nodiscard]] auto isLeaf() const -> bool override;
  [[nodiscard]] auto getShape() -> Shape &;

private:
  std::unique_ptr<Shape> shape;
};

class CSGTree {
public:
  std::unique_ptr<CSGNode> root = nullptr;
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

  [[nodiscard]] auto getLeftChild() -> CSGNode &;
  [[nodiscard]] auto getRightChild() -> CSGNode &;
  [[nodiscard]] auto isLeaf() const -> bool override;
  [[nodiscard]] auto getOperation() -> Operation &;

private:
  auto internalSetLeftChild(std::unique_ptr<CSGNode> &&leftChild) -> CSGNode &;
  auto internalSetRightChild(std::unique_ptr<CSGNode> &&rightChild) -> CSGNode &;

  std::unique_ptr<CSGNode> leftChild;
  std::unique_ptr<CSGNode> rightChild;
  std::unique_ptr<Operation> operation;
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
