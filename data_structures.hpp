#pragma once

#include <cstdint>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

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

/**
 * @brief Return a pointer n_bytes of memory aligned to at least n_align bytes
 *
 * @tparam T
 * @param n_align - Number of bytes for alignment
 * @param n_bytes - Number of bytes to allocate
 * @return T* - Aligned pointer or nullptr on failure
 */
template <typename T>
T *aligned_alloc(std::size_t n_align, std::size_t n_bytes) {
  /* Internally we allocate a block that stores
    - sizeof(T*) - Original pointer needed to actually deallocate
    - Padding to satisfy alignment
    - Object of type T aligned to n_align
   */
  // TODO can I shrink this size?
  // TODO what about alignment of T and void*?
  std::size_t n_bytes_alloc = n_bytes + n_align - 1 + sizeof(void *);
  void *base_ptr = std::malloc(n_bytes_alloc);
  if (base_ptr == nullptr) {
    return nullptr;
  }
  auto base_ptr_val = reinterpret_cast<std::uintptr_t>(base_ptr);
  auto offset = n_align - (base_ptr_val % n_align);
  void **obj_ptr = reinterpret_cast<void **>(base_ptr_val + offset);
  obj_ptr[-1] = base_ptr;
  return reinterpret_cast<T *>(obj_ptr);

  // Inspiration from
  // https://stackoverflow.com/questions/38088732/explanation-to-aligned-malloc-implementation
  // void *p1;  // original block
  // void **p2; // aligned block
  // int offset = n_align - 1 + sizeof(void *);
  // if ((p1 = (void *)malloc(n_bytes + offset)) == NULL) {
  //   return NULL;
  // }
  // p2 = (void **)(((size_t)(p1) + offset) & ~(n_align - 1));
  // p2[-1] = p1;
  // return p2;
}

inline void aligned_free(void *ptr) {
  if (ptr == nullptr) {
    return;
  }
  std::free(reinterpret_cast<void **>(ptr)[-1]);
}

struct bad_heap_access : std::runtime_error {
  bad_heap_access(std::string msg) : std::runtime_error(msg) {}
};
template <typename T> struct heap {
  heap() {}
  heap(std::vector<T> &&data) : fData(std::move(data)) {
    heapify();
  }

  using size_type = std::size_t;
  size_type size() const {
    return fData.size();
  }

  bool empty() const {
    return size() == 0;
  }

  const T &peek() const {
    check_not_empty();
    return fData[0];
  }

  T &peek() {
    check_not_empty();
    return fData[0];
  }

  void insert(T val) {
    this->fData.push_back(val);
    this->sift_up(size() - 1);
  };

  T pop() {
    check_not_empty();
    auto res = this->fData[0];
    this->fData[0] = this->fData.back();
    this->fData.pop_back();
    return res;
  };

  void delete_extremum() {
    (void)pop();
  }

private:
  void check_not_empty() const {
    if (this->fData.empty()) {
      throw bad_heap_access("Attempted to access an element of an empty heap");
    }
  }
  void sift_up(std::size_t el_idx) {
    auto parent_idx = (el_idx - 1) / 2;
    while (el_idx > 0 && this->fData[el_idx] > this->fData[parent_idx]) {
      std::swap(this->fData[el_idx], this->fData[parent_idx]);
      el_idx = parent_idx;
      parent_idx = (el_idx - 1) / 2;
    }
  }

  void sift_down(std::size_t el_idx) {
    while (true) {
      auto left_idx = 2 * el_idx + 1;
      auto right_idx = 2 * el_idx + 2;
      auto max_idx = el_idx;
      if (left_idx < size() && this->fData[max_idx] < this->fData[left_idx]) {
        max_idx = left_idx;
      }
      if (right_idx < size() && this->fData[max_idx] < this->fData[right_idx]) {
        max_idx = right_idx;
      }
      if (max_idx == el_idx) {
        return;
      }
      std::swap(this->fData[el_idx], this->fData[max_idx]);
      el_idx = max_idx;
    }
  }

  void heapify() {
    if (this->empty()) {
      return;
    }
    auto n = size();
    auto non_leaf = (n - 1) / 2;
    for (std::size_t el_idx{non_leaf + 1}; el_idx > 0; --el_idx) {
      // Sift downwards to achieve heap property
      sift_down(el_idx - 1);
    }
  }

  std::vector<T> fData;
};
} // namespace ry
