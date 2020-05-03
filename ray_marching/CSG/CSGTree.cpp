//
// Created by petr on 4/4/20.
//

#include "CSGTree.h"
#include <fmt/format.h>
#include <iostream>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>
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
auto OperationCSGNode::eval(float d1, float d2) -> float { return operation->eval(d1, d2); }
auto OperationCSGNode::isBinary() const -> bool { return true; }
auto OperationCSGNode::setLeftChild(std::unique_ptr<CSGNode> &&child) -> CSGNode & {
  leftChild = std::move(child);
  return *leftChild;
}
auto OperationCSGNode::setRightChild(std::unique_ptr<CSGNode> &&child) -> CSGNode & {
  rightChild = std::move(child);
  return *rightChild;
}
auto OperationCSGNode::getOperation() const -> const BinaryOperation & { return *operation; }
auto OperationCSGNode::getCSGData() -> CSGRawData & { return getOperation(); }
auto OperationCSGNode::getCSGData() const -> const CSGRawData & { return getOperation(); }
auto OperationCSGNode::getLeftChild() const -> const CSGNode & { return *leftChild; }
auto OperationCSGNode::getRightChild() const -> const CSGNode & { return *rightChild; }

ShapeCSGNode::ShapeCSGNode(std::unique_ptr<Shape> &&shape) : shape(std::move(shape)) {}
auto ShapeCSGNode::isLeaf() const -> bool { return true; }
auto ShapeCSGNode::getShape() -> Shape & { return *shape; }
auto ShapeCSGNode::eval(const glm::vec3 &camPos) -> float { return shape->distance(camPos); }
auto ShapeCSGNode::isBinary() const -> bool { return false; }
auto ShapeCSGNode::getShape() const -> const Shape & { return *shape; }
auto ShapeCSGNode::getCSGData() -> CSGRawData & { return getShape(); }
auto ShapeCSGNode::getCSGData() const -> const CSGRawData & { return getShape(); }

auto CSGTree::src() -> std::string {
  std::string result = "{}";
  csgInorder(*root, overload{[&result](OperationCSGNode &node) { result = replace(result, "{}", node.getOperation().src()); },
                             [&result](WarpOperationCSGNode &node) { result = replace(result, "{}", node.getOperation().src()); },
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
    auto &warpOperationNode = reinterpret_cast<WarpOperationCSGNode &>(node);
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
CSGTree::CSGTree(std::string name) : name(std::move(name)) {}
auto WarpOperationCSGNode::internalSetChild(std::unique_ptr<CSGNode> &&child) -> CSGNode & {
  WarpOperationCSGNode::child = std::move(child);
  return *WarpOperationCSGNode::child;
}
auto WarpOperationCSGNode::isLeaf() const -> bool { return false; }
auto WarpOperationCSGNode::eval(const glm::vec3 &camPos) -> glm::vec3 { return operation->eval(camPos); }
auto WarpOperationCSGNode::getChild() -> CSGNode & { return *child; }
auto WarpOperationCSGNode::isBinary() const -> bool { return false; }
auto WarpOperationCSGNode::getOperation() -> SpaceWarpOperation & { return *operation; }
auto WarpOperationCSGNode::setChild(std::unique_ptr<CSGNode> &&child) -> CSGNode & {
  WarpOperationCSGNode::child = std::move(child);
  return *WarpOperationCSGNode::child;
}
auto WarpOperationCSGNode::getOperation() const -> const SpaceWarpOperation & { return *operation; }
auto WarpOperationCSGNode::getCSGData() -> CSGRawData & { return getOperation(); }
auto WarpOperationCSGNode::getCSGData() const -> const CSGRawData & { return getOperation(); }
auto WarpOperationCSGNode::getChild() const -> const CSGNode & { return *child; }

auto CSGTree::FromJson(const nlohmann::json &json) -> std::optional<std::unique_ptr<CSGTree>> {
  using namespace std::string_literals;
  if (!json.contains("id") || !json.contains("csg")) {
    spdlog::error("Invalid CSG object: "s + json.dump(2));
    return std::nullopt;
  }

  auto tree = std::make_unique<CSGTree>(std::string(json["id"]));
  const auto csgJson = json["csg"];
  auto root = NodeFromJson(csgJson);
  if (!root.has_value()) {
    return std::nullopt;
  }
  tree->root = std::move(root.value());
  return tree;
}

auto CSGTree::NodeFromJson(const nlohmann::json &json) -> std::optional<std::unique_ptr<CSGNode>> {
  using namespace std::string_literals;
  if (!json.contains("nodeType")) {
    spdlog::error("Invalid CSG object: "s + json.dump(2));
    return std::nullopt;
  }
  const auto nodeType = magic_enum::enum_cast<NodeType>(std::string(json["nodeType"]));
  if (!nodeType.has_value()) {
    spdlog::error("Invalid CSG node type: "s + json.dump(2));
    return std::nullopt;
  }
  switch (nodeType.value()) {
  case NodeType::WarpOperation:
    return WarpNodeFromJson(json);
  case NodeType::Operation:
    return OperationNodeFromJson(json);
  case NodeType::Shape:
    return ShapeNodeFromJson(json);
  }
  return std::nullopt;
}

auto CSGTree::WarpNodeFromJson(const nlohmann::json &json) -> std::optional<std::unique_ptr<WarpOperationCSGNode>> {
  using namespace std::string_literals;
  if (!json.contains("operationType")) {
    spdlog::error("Invalid warp operation: "s + json.dump(2));
    return std::nullopt;
  }
  auto result = std::unique_ptr<WarpOperationCSGNode>{nullptr};
  if (json["operationType"] == "LimitedSpaceRepetition") {
    if (!json.contains("domain") || !json.contains("limit")) {
      spdlog::error("Missing parameters warp operation: "s + json.dump(2));
      return std::nullopt;
    }
    const auto domain = glm::vec3{json["domain"]["x"], json["domain"]["y"], json["domain"]["z"]};
    const auto limit = glm::vec3{json["limit"]["x"], json["limit"]["y"], json["limit"]["z"]};
    result = std::make_unique<WarpOperationCSGNode>(std::make_unique<LimitedSpaceRepetitionOperation>(domain, limit));
  }
  if (json["operationType"] == "SpaceRepetition") {
    if (!json.contains("domain")) {
      spdlog::error("Missing parameters warp operation: "s + json.dump(2));
      return std::nullopt;
    }
    const auto domain = glm::vec3{json["domain"]["x"], json["domain"]["y"], json["domain"]["z"]};
    result = std::make_unique<WarpOperationCSGNode>(std::make_unique<SpaceRepetitionOperation>(domain));
  }
  if (json["operationType"] == "SpaceStretch") {
    if (!json.contains("multiplier")) {
      spdlog::error("Missing parameters warp operation: "s + json.dump(2));
      return std::nullopt;
    }
    const auto multiplier = glm::vec3{json["multiplier"]["x"], json["multiplier"]["y"], json["multiplier"]["z"]};
    result = std::make_unique<WarpOperationCSGNode>(std::make_unique<SpaceStretchOperation>(multiplier));
  }
  if (result == nullptr) {
    spdlog::error("Invalid warp operation type: "s + json.dump(2));
    return std::nullopt;
  }
  if (!json.contains("operand")) {
    spdlog::error("Missing operand for operation: "s + json.dump(2));
    return std::nullopt;
  }
  auto child = NodeFromJson(json["operand"]);
  if (!child.has_value()) {
    return std::nullopt;
  }
  result->setChild(std::move(child.value()));
  return result;
}

auto CSGTree::OperationNodeFromJson(const nlohmann::json &json) -> std::optional<std::unique_ptr<OperationCSGNode>> {
  using namespace std::string_literals;
  if (!json.contains("operationType")) {
    spdlog::error("Invalid warp operation: "s + json.dump(2));
    return std::nullopt;
  }
  auto result = std::unique_ptr<OperationCSGNode>{nullptr};
  if (json["operationType"] == "Union") {
    result = std::make_unique<OperationCSGNode>(std::make_unique<OperationUnion>());
  }
  if (json["operationType"] == "Substraction") {
    result = std::make_unique<OperationCSGNode>(std::make_unique<OperationSubstraction>());
  }
  if (json["operationType"] == "Blend") {
    if (!json.contains("k")) {
      spdlog::error("Missing parameters for operation: "s + json.dump(2));
      return std::nullopt;
    }
    result = std::make_unique<OperationCSGNode>(std::make_unique<OperationBlend>(json["k"]));
  }
  if (result == nullptr) {
    spdlog::error("Invalid operation type: "s + json.dump(2));
    return std::nullopt;
  }
  if (!json.contains("leftOperand") || !json.contains("rightOperand")) {
    spdlog::error("Missing operands for operation: "s + json.dump(2));
    return std::nullopt;
  }
  auto leftChild = NodeFromJson(json["leftOperand"]);
  if (!leftChild.has_value()) {
    return std::nullopt;
  }
  auto rightChild = NodeFromJson(json["rightOperand"]);
  if (!rightChild.has_value()) {
    return std::nullopt;
  }
  result->setLeftChild(std::move(leftChild.value()));
  result->setRightChild(std::move(rightChild.value()));
  return result;
}

auto CSGTree::ShapeNodeFromJson(const nlohmann::json &json) -> std::optional<std::unique_ptr<ShapeCSGNode>> {
  using namespace std::string_literals;
  if (!json.contains("shapeType")) {
    spdlog::error("Invalid warp operation: "s + json.dump(2));
    return std::nullopt;
  }
  auto result = std::unique_ptr<ShapeCSGNode>{nullptr};
  if (!json.contains("position")) {
    spdlog::error("Missing position for shape: "s + json.dump(2));
    return std::nullopt;
  }
  const auto position = glm::vec3{json["position"]["x"], json["position"]["y"], json["position"]["z"]};
  if (json["shapeType"] == "Box") {
    if (!json.contains("dimensions")) {
      spdlog::error("Missing parameters for operation: "s + json.dump(2));
      return std::nullopt;
    }
    const auto dimensions = glm::vec3{json["dimensions"]["x"], json["dimensions"]["y"], json["dimensions"]["z"]};
    result = std::make_unique<ShapeCSGNode>(std::make_unique<BoxShape>(position, dimensions));
  }
  if (json["shapeType"] == "Sphere") {
    if (!json.contains("radius")) {
      spdlog::error("Missing parameters for operation: "s + json.dump(2));
      return std::nullopt;
    }
    const float radius = json["radius"];
    result = std::make_unique<ShapeCSGNode>(std::make_unique<SphereShape>(position, radius));
  }
  if (json["shapeType"] == "Plane") {
    if (!json.contains("normal")) {
      spdlog::error("Missing parameters for operation: "s + json.dump(2));
      return std::nullopt;
    }
    const auto normal = glm::vec4{json["normal"]["x"], json["normal"]["y"], json["normal"]["z"], json["normal"]["w"]};
    result = std::make_unique<ShapeCSGNode>(std::make_unique<PlaneShape>(position, normal));
  }
  if (result == nullptr) {
    spdlog::error("Invalid shape type: "s + json.dump(2));
    return std::nullopt;
  }
  return result;
}

template <typename F> void createRawData(const CSGNode &node, CSGTree::Raw &result, F &&appendNode, uint32_t &nodeCnt) {
  if (node.isLeaf()) {
    return;
  } else if (node.isBinary()) {
    const auto &opNode = reinterpret_cast<const OperationCSGNode &>(node);
    const auto index = nodeCnt - 1;
    const auto &rightChild = opNode.getRightChild();
    const auto &leftChild = opNode.getLeftChild();
    const auto rightChildIndex = appendNode(rightChild);
    createRawData(rightChild, result, appendNode, nodeCnt);
    const auto leftChildIndex = appendNode(leftChild);
    createRawData(leftChild, result, appendNode, nodeCnt);
    const auto childrenIndices = rightChildIndex << 16u | leftChildIndex;
    result.treeData[index * 3 + 2] = childrenIndices;
  } else {
    const auto &opNode = reinterpret_cast<const WarpOperationCSGNode &>(node);
    const auto index = nodeCnt - 1;
    const auto &child = opNode.getChild();
    const auto childIndex = appendNode(child);
    createRawData(child, result, appendNode, nodeCnt);
    result.treeData[index * 3 + 2] = childIndex;
  }
}

void createPostOrderIndices(CSGTree::Raw &raw) {
  constexpr std::size_t step = 3;
  raw.postOrderTraversal.clear();
  raw.postOrderTraversal.emplace_back(raw.treeData.size() / step);
  // for (int i = raw.treeData.size() / step - 1; i >= 0;  --i) {
  for (int i = 0; i < raw.treeData.size() / step; ++i) {
    raw.postOrderTraversal.emplace_back(i);
  }
}

auto CSGTree::raw(std::size_t nodeOffset, std::size_t paramOffset) const -> CSGTree::Raw {
  Raw result{};
  uint32_t nodeCnt = nodeOffset;
  auto appendData = [](auto &paramData, const auto &parameters) {
    paramData.insert(paramData.end(), parameters.begin(), parameters.end());
  };
  auto appendNode = [&appendData, &paramOffset, &result, &nodeCnt](const auto &node) -> uint32_t {
    const auto treeData = std::array<uint32_t, 3>{node.getCSGData().rawTypeInfo(), static_cast<unsigned int>(paramOffset), 0};
    const auto parameters = node.getCSGData().rawParameters();
    appendData(result.treeData, treeData);
    appendData(result.paramData, parameters);
    paramOffset += parameters.size();
    return nodeCnt++;
  };
  appendNode(*root);
  createRawData(*root, result, appendNode, nodeCnt);
  createPostOrderIndices(result);
  return result;
}
