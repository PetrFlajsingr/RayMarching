
template <typename T, unsigned int ChildCount> Tree<T, ChildCount>::Tree(value_type rootValue)
    : root(std::make_unique<Root>(rootValue)) {}
template <typename T, unsigned int ChildCount>
Tree<T, ChildCount> Tree<T, ChildCount>::BuildTree(std::size_t depth, const_reference_type initValue) {
  Tree result{initValue};
  initChildren(result.root.get(), initValue, depth - 1);
  return std::move(result);
}
template <typename T, unsigned int ChildCount> template <typename F> void Tree<T, ChildCount>::traverseDepthFirst(F &&callable) {
  root->traverseDepthFirst(callable);
}
template <typename T, unsigned int ChildCount> template <typename F>
void Tree<T, ChildCount>::traverseBreadthFirst(F &&callable) {
  root->traverseBreadthFirst(callable);
}
template <typename T, unsigned int ChildCount> template <typename F>
void Tree<T, ChildCount>::traverseDepthFirstIf(F &&callable) {
  root->traverseDepthFirstIf(callable);
}
template <typename T, unsigned int ChildCount> template <typename F, unsigned int C, typename>
void Tree<T, ChildCount>::preorder(F &&callable) {
  detail::preorderImpl(root.get(), callable);
}
template <typename T, unsigned int ChildCount> template <typename F, unsigned int C, typename>
void Tree<T, ChildCount>::inorder(F &&callable) {
  detail::inorderImpl(root.get(), callable);
}
template <typename T, unsigned int ChildCount> template <typename F, unsigned int C, typename>
void Tree<T, ChildCount>::postorder(F &&callable) {
  detail::postorderImpl(root.get(), callable);
}
template <typename T, unsigned int ChildCount>
void Tree<T, ChildCount>::initChildren(Node<T, ChildCount> *node, const_reference_type initValue, std::size_t depth) {
  NodeType nodeType = NodeType::Node;
  if (depth == 0) {
    nodeType = NodeType::Leaf;
  }
  node->setChildrenValues(initValue, nodeType);
  if (nodeType == NodeType::Leaf) {
    return;
  }
  for (auto &child : node->getChildren()) {
    initChildren(&child->asNode(), initValue, depth - 1);
  }
}
template <typename T, unsigned int ChildCount> Tree<T, ChildCount>::Tree(const Tree &other) {
  root = other.root == nullptr ? nullptr : std::make_unique<Root>(other.root->asNode());
}
template <typename T, unsigned int ChildCount> Tree<T, ChildCount> &Tree<T, ChildCount>::operator=(const Tree &other) {
  if (&other == this) {
    return *this;
  }
  root = other.root == nullptr ? nullptr : std::make_unique<Root>(other.root->asNode());
  return *this;
}
