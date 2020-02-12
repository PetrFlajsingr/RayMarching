
template <typename T, unsigned int ChildCount, typename F>
void detail::traverseDepthFirstImpl(Leaf<T, ChildCount> *node, F &callable) {
  if (node == nullptr) {
    return;
  }
  callable(node->getValue());
  if (node->getType() == NodeType::Leaf) {
    return;
  }
  auto notLeafNode = reinterpret_cast<Node<T, ChildCount> *>(node);
  for (auto &child : notLeafNode->getChildren()) {
    traverseDepthFirstImpl(child.get(), callable);
  }
}
template <typename T, unsigned int ChildCount, typename F>
void detail::traverseDepthFirstIfImpl(Leaf<T, ChildCount> *node, F &callable) {
  if (node == nullptr) {
    return;
  }
  const bool shouldContinue = callable(node->getValue());
  if (node->getType() == NodeType::Leaf || !shouldContinue) {
    return;
  }
  auto notLeafNode = reinterpret_cast<Node<T, ChildCount> *>(node);
  for (auto &child : notLeafNode->getChildren()) {
    traverseDepthFirstIfImpl(child.get(), callable);
  }
}
template <typename T, unsigned int ChildCount, typename F>
void detail::traverseBreadthFirstImpl(Leaf<T, ChildCount> *node, F &callable) {
  if (node == nullptr) {
    return;
  }
  if (node->getType() == NodeType::Leaf) {
    return;
  }

  std::queue<Node<T, ChildCount> *> queue;
  queue.push(reinterpret_cast<Node<T, ChildCount> *>(node));

  while (!queue.empty()) {
    auto currentNode = queue.front();
    queue.pop();
    if (currentNode == nullptr) {
      continue;
    }
    callable(currentNode->getValue());
    if (currentNode->getType() == NodeType::Leaf) {
      continue;
    }
    for (auto &child : currentNode->getChildren()) {
      queue.push(reinterpret_cast<Node<T, ChildCount> *>(child.get()));
    }
  }
}
template <typename T, typename F> void detail::preorderImpl(Leaf<T, 2> *node, F &&callable) {
  if (node == nullptr) {
    return;
  }
  callable(node->getValue());
  if (node->getType() == NodeType::Leaf) {
    return;
  }
  auto notLeafNode = reinterpret_cast<Node<T, 2> *>(node);
  preorderImpl(&notLeafNode->leftChild(), callable);
  preorderImpl(&notLeafNode->rightChild(), callable);
}
template <typename T, typename F> void detail::inorderImpl(Leaf<T, 2> *node, F &&callable) {
  if (node == nullptr) {
    return;
  }
  auto notLeafNode = reinterpret_cast<Node<T, 2> *>(node);
  if (node->getType() != NodeType::Leaf) {
    preorderImpl(&notLeafNode->leftChild(), callable);
  }
  callable(node->getValue());
  if (node->getType() != NodeType::Leaf) {
    preorderImpl(&notLeafNode->rightChild(), callable);
  }
}
template <typename T, typename F> void detail::postorderImpl(Leaf<T, 2> *node, F &&callable) {
  if (node == nullptr) {
    return;
  }
  auto notLeafNode = reinterpret_cast<Node<T, 2> *>(node);
  if (node->getType() != NodeType::Leaf) {
    preorderImpl(&notLeafNode->leftChild(), callable);
  }
  if (node->getType() != NodeType::Leaf) {
    preorderImpl(&notLeafNode->rightChild(), callable);
  }
  callable(node->getValue());
}


template <typename T, unsigned int ChildCount> TreeIterator<T, ChildCount>::TreeIterator() : root(nullptr), current(nullptr) {}
template <typename T, unsigned int ChildCount> TreeIterator<T, ChildCount>::TreeIterator(TreeIterator::LeafType *root)
    : root(root), current(root) {
  buildQueue(root);
  operator++();
}
template <typename T, unsigned int ChildCount> TreeIterator<T, ChildCount>::TreeIterator(const TreeIterator::iterator &other)
    : root(other.root), current(other.root), queue(other.queue) {}
template <typename T, unsigned int ChildCount>
typename TreeIterator<T, ChildCount>::iterator &TreeIterator<T, ChildCount>::operator=(const TreeIterator::iterator &other) {
  if (&other == this) {
    return *this;
  }
  root = other.root;
  current = other.root;
  queue = other.queue;
  return *this;
}
template <typename T, unsigned int ChildCount>
bool TreeIterator<T, ChildCount>::operator==(const TreeIterator::iterator &other) const {
  return current == other.current;
}
template <typename T, unsigned int ChildCount>
bool TreeIterator<T, ChildCount>::operator!=(const TreeIterator::iterator &other) const {
  return !(*this == other);
}
template <typename T, unsigned int ChildCount>
typename TreeIterator<T, ChildCount>::reference TreeIterator<T, ChildCount>::operator*() {
  return current->getValue();
}
template <typename T, unsigned int ChildCount>
typename TreeIterator<T, ChildCount>::pointer TreeIterator<T, ChildCount>::operator->() {
  return &current->getValue();
}
template <typename T, unsigned int ChildCount>
typename TreeIterator<T, ChildCount>::iterator &TreeIterator<T, ChildCount>::operator++() {
  if (queue.empty()) {
    current = nullptr;
  } else {
    current = queue.front();
    queue.pop();
  }
  return *this;
}
template <typename T, unsigned int ChildCount>
typename TreeIterator<T, ChildCount>::iterator TreeIterator<T, ChildCount>::operator++(int) {
  auto result = *this;
  ++*this;
  return result;
}
template <typename T, unsigned int ChildCount> void TreeIterator<T, ChildCount>::buildQueue(TreeIterator::LeafType *node) {
  if (node == nullptr) {
    return;
  }
  queue.push(node);
  if (node->getType() == NodeType::Leaf) {
    return;
  }
  auto notLeafNode = reinterpret_cast<Node<T, ChildCount> *>(node);
  for (auto &child : notLeafNode->getChildren()) {
    buildQueue(child.get());
  }
}
