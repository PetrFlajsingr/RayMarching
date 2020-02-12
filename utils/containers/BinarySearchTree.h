//
// Created by Petr on 04.02.2020.
//

#ifndef UTILITIES_BINARYSEARCHTREE_H
#define UTILITIES_BINARYSEARCHTREE_H

#include "error_handling/exceptions.h"

template <typename KeyType, typename ValueType> class BinarySearchTreeIterator;

template <typename KeyType, typename ValueType> class BinarySearchTreeNode {
public:
  using value_type = ValueType;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using iterator = BinarySearchTreeIterator<KeyType, ValueType>;

  BinarySearchTreeNode(const KeyType &key, const ValueType &value) : key(key), value(value) {}

  reference operator*() { return value; }
  const_reference operator*() const { return value; }

  pointer operator->() { return &value; }
  const_pointer operator->() const { return &value; }

  BinarySearchTreeNode *getLeftChild() { return leftChild.get(); }
  BinarySearchTreeNode *getRightChild() { return rightChild.get(); }

  void setLeftChild(const KeyType &key, const_reference value) {
    if (leftChild == nullptr) {
      leftChild = std::make_unique<BinarySearchTreeNode>(key, value);
    } else {
      leftChild->setKey(key);
      leftChild->setValue(value);
    }
  }

  void setRightChild(const KeyType &key, const_reference value) {
    if (rightChild == nullptr) {
      rightChild = std::make_unique<BinarySearchTreeNode>(key, value);
    } else {
      rightChild->setKey(key);
      rightChild->setValue(value);
    }
  }

  void setValue(const_reference value) { this->value = value; }

  void setKey(const KeyType &key) { this->key = key; }

  const KeyType &getKey() const { return key; }

  reference getValue() { return value; }

  iterator begin() { return iterator{this}; }
  iterator end() { return iterator::End(this); }

private:
  KeyType key;
  value_type value;

  std::unique_ptr<BinarySearchTreeNode> leftChild;
  std::unique_ptr<BinarySearchTreeNode> rightChild;
};

template <typename KeyType, typename ValueType> class BinarySearchTreeIterator {
public:
  using iterator = BinarySearchTreeIterator;
  using value_type = std::pair<KeyType &, ValueType &>;
  using reference = value_type &;
  using pointer = value_type *;
  using difference_type = std::size_t;
  using iterator_category = std::input_iterator_tag;

  BinarySearchTreeIterator() = default;
  explicit BinarySearchTreeIterator(BinarySearchTreeNode<KeyType, ValueType> *root) : root(root) { buildQueue(root); }

  static BinarySearchTreeIterator End(BinarySearchTreeNode<KeyType, ValueType> *root) {
    BinarySearchTreeIterator result;
    result.root = root;
    return result;
  }

  BinarySearchTreeIterator(const iterator &other) {
    queue = other.queue;
    root = other.root;
  }
  iterator &operator=(const iterator &other) {
    if (this == &other) {
      return *this;
    }
    queue = other.queue;
    root = other.root;
    return *this;
  }

  bool operator==(const iterator &other) const { return root == other.root && queue.size() == other.queue.size(); }
  bool operator!=(const iterator &other) const { return !(*this == other); }

  struct NodeDataRef {
    const KeyType &key;
    ValueType &value;
  };

  NodeDataRef operator*() { return NodeDataRef{queue.front()->getKey(), queue.front()->getValue()}; }

  iterator &operator++() {
    if (!queue.empty()) {
      queue.pop();
    }
    return *this;
  }
  iterator operator++(int) {
    auto result = *this;
    ++*this;
    return result;
  }

private:
  std::queue<BinarySearchTreeNode<KeyType, ValueType> *> queue;
  BinarySearchTreeNode<KeyType, ValueType> *root;

  void buildQueue(BinarySearchTreeNode<KeyType, ValueType> *node) {
    if (node == nullptr) {
      return;
    }
    if (node->getLeftChild() != nullptr) {
      buildQueue(node->getLeftChild());
    }
    queue.push(node);
    if (node->getRightChild() != nullptr) {
      buildQueue(node->getRightChild());
    }
  }
};

template <typename KeyType, typename ValueType> class BinarySearchTree {
public:
  using iterator = BinarySearchTreeIterator<KeyType, ValueType>;

  BinarySearchTreeNode<KeyType, ValueType> &insert(const KeyType &key, const ValueType &value) {
    BinarySearchTreeNode<KeyType, ValueType> *tmp = root.get();
    while (tmp) {
      if (tmp->getKey() == key) {
        tmp->setValue(value);
        return *tmp;
      }
      if (key > tmp->getKey()) {
        if (tmp->getRightChild() == nullptr) {
          tmp->setRightChild(key, value);
          return *tmp->getRightChild();
        }
        tmp = tmp->getRightChild();
      } else {
        if (tmp->getLeftChild() == nullptr) {
          tmp->setLeftChild(key, value);
          return *tmp->getLeftChild();
        }
        tmp = tmp->getLeftChild();
      }
    }
    root = std::make_unique<BinarySearchTreeNode<KeyType, ValueType>>(key, value);
    return *root;
  }

  void remove(const KeyType &key) {
    // TODO
  }

  void erase(iterator position) {
    // TODO
  }

  iterator begin() {
    if (root == nullptr) {
      return iterator::End(nullptr);
    }
    return iterator(root.get());
  }

  iterator end() {
    if (root == nullptr) {
      return iterator::End(nullptr);
    }
    return iterator::End(root.get());
  }

private:
  std::unique_ptr<BinarySearchTreeNode<KeyType, ValueType>> root;
};

#endif // UTILITIES_BINARYSEARCHTREE_H
