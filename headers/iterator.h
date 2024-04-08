#pragma once

#include "node.h"
#include <iterator>

namespace HomeTask {
template <typename T, typename It> class BaseIterator {
public:
  explicit BaseIterator(Node<T> &node_ptr) : _ptr(&node_ptr){};
  explicit BaseIterator(const Node<T> &node_ptr)
      : _ptr(&(std::remove_const_t<Node<T> &>(node_ptr))){};

  explicit BaseIterator(const BaseIterator &it) { _ptr = it._ptr; }

  virtual ~BaseIterator() = default;

public:
  Node<T> *_ptr;

public:
  It operator++() {
    _ptr = _ptr->next;
    return static_cast<It &>(*this);
  }

  It operator++(int) {
    It result = It(*this->_ptr);
    _ptr = _ptr->next;
    return result;
  }

  It operator--() {
    _ptr = _ptr->prev;
    return static_cast<It &>(*this);
  }

  It operator--(int) {
    It result = It(*(this->_ptr));
    _ptr = _ptr->prev;
    return result;
  }

  It &operator=(const BaseIterator<T, It> &other) {
    _ptr = other._ptr;
    return static_cast<It &>(*this);
  }

  It &operator=(const BaseIterator<T, It> &&other) {
    _ptr = other._ptr;
    other._ptr = nullptr;
    return static_cast<It &>(*this);
  }

  bool operator!=(const BaseIterator<T, It> &it) { return !(*this == it); }

  bool operator==(const BaseIterator<T, It> &it) { return (_ptr == it._ptr); }

public:
  void swap(BaseIterator &it) { std::swap(_ptr, it._ptr); }
};

template <typename T> class Iterator : public BaseIterator<T, Iterator<T>> {
public:
  explicit Iterator(const Node<T> &node_ptr)
      : BaseIterator<T, Iterator>(node_ptr) {}

public:
  using BaseIterator<T, Iterator>::_ptr;

  T &operator*() { return this->_ptr->getData(); }
  T *operator->() { return &(this->_ptr->getData()); }
};

template <typename T>
class ConstIterator : public BaseIterator<T, ConstIterator<T>> {
public:
  explicit ConstIterator(const Node<T> &node_ptr)
      : BaseIterator<T, ConstIterator>(node_ptr) {}

public:
  using BaseIterator<T, ConstIterator<T>>::_ptr;

  const T &operator*() { return this->_ptr->getData(); }
  const T *operator->() { return &(this->_ptr->getData()); }
};
} // namespace HomeTask