#pragma once

#include <memory>

namespace ry {
template <typename T> struct list_node;

template <typename T> using list_ptr_t = std::shared_ptr<list_node<T>>;

template <typename T> struct list_node {
  list_node(T x, list_ptr_t<T> n = nullptr) : data(x), next(n) {}

  T data;
  list_ptr_t<T> next = nullptr;
};

template <typename T> struct list_iterator {
  list_iterator(list_ptr_t<T> in_node) : fNode(in_node) {}

  list_iterator<T> &operator++() {
    this->fNode = this->fNode->next;
    return *this;
  }

  list_iterator<T> operator++(int) {
    auto copy = *this;
    this->fNode = this->fNode->next;
    return copy;
  }

  T &operator*() {
    return this->fNode->data;
  }

  T *operator->() {
    return &this->fNode->data;
  }

  bool operator==(const list_iterator<T> &other) {
    return other.fNode == this->fNode;
  }

  bool operator!=(const list_iterator<T> &other) {
    return !(*this == other);
  }

private:
  list_ptr_t<T> fNode{nullptr};
};

template <typename T> struct list {
  using size_type = std::size_t;

  template <typename IterableT> list(IterableT l) : fSize(l.size()) {
    list_ptr_t<T> prev = nullptr;
    for (auto item : l) {
      auto node = std::make_shared<list_node<T>>(item);
      if (prev) {
        prev->next = node;
      } else {
        fHead = node;
      }
      prev = node;
    }
  }

  list_iterator<T> begin() {
    return list_iterator<T>(this->fHead);
  }

  list_iterator<T> end() {
    return list_iterator<T>(nullptr);
  }

  size_type size() const {
    return fSize;
  }

  void reverse() {
    list_ptr_t<T> prev, current{this->fHead}, next;
    while (current) {
      next = current->next;
      current->next = prev;
      prev = current;
      current = next;
    }
  }

private:
  size_type fSize{0};
  list_ptr_t<T> fHead = nullptr;
};

template <typename T> std::ostream &operator<<(std::ostream &os, list<T> l) {
  auto it = l.begin();
  os << "[" << *it;
  ++it;

  for (; it != l.end(); ++it) {
    os << "," << *it;
  }

  os << "]";

  return os;
}
} // namespace ry
