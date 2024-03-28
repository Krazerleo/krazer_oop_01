#pragma once

#include "node.h"
#include <iterator>

namespace HomeTask {
template <typename T, typename It> class BaseIterator {
public:
  using difference_type = ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;
  using pointer = T *;
  using reference = T &;

public:
  BaseNode<T> *_curr_ptr;

public:
  explicit BaseIterator(BaseNode<T> &node_ptr) : _curr_ptr(&node_ptr){};
  explicit BaseIterator(const BaseNode<T> &node_ptr)
      : _curr_ptr(&(std::remove_const_t<BaseNode<T> &>(node_ptr))){};

  explicit BaseIterator(const BaseIterator &it) { _curr_ptr = it._curr_ptr; }

  virtual ~BaseIterator() = default;

public:
  It operator++() {
    _curr_ptr = _curr_ptr->next;
    return static_cast<It &>(*this);
  }

  It operator++(int) {
    It result = It(*this->_curr_ptr);
    _curr_ptr = _curr_ptr->next;
    return result;
  }

  It operator--() {
    _curr_ptr = _curr_ptr->prev;
    return static_cast<It &>(*this);
  }

  It operator--(int) {
    It result = It(*(this->_curr_ptr));
    _curr_ptr = _curr_ptr->prev;
    return result;
  }

  It &operator=(const BaseIterator<T, It> &other) {
    _curr_ptr = other._curr_ptr;
    return static_cast<It &>(*this);
  }

  It &operator=(const BaseIterator<T, It> &&other) {
    _curr_ptr = other._curr_ptr;
    other._curr_ptr = nullptr;
    return static_cast<It &>(*this);
  }

  bool operator!=(const BaseIterator<T, It> &it) { return !(*this == it); }

  bool operator==(const BaseIterator<T, It> &it) {
    return (_curr_ptr == it._curr_ptr);
  }

public:
  void swap(BaseIterator &it) { std::swap(_curr_ptr, it._curr_ptr); }
};

template <typename T> class Iterator : public BaseIterator<T, Iterator<T>> {
public:
  explicit Iterator(const BaseNode<T> &node_ptr)
      : BaseIterator<T, Iterator>(node_ptr) {}

public:
  using BaseIterator<T, Iterator>::_curr_ptr;
  T &operator*() { return this->_curr_ptr->getData(); }
  T *operator->() { return &(this->_curr_ptr->getData()); }
};

template <typename T>
class ConstIterator : public BaseIterator<T, ConstIterator<T>> {
public:
  explicit ConstIterator(const BaseNode<T> &node_ptr)
      : BaseIterator<T, ConstIterator>(node_ptr) {}

public:
  using BaseIterator<T, ConstIterator<T>>::_curr_ptr;
  const T &operator*() { return this->_curr_ptr->getData(); }
  const T *operator->() { return &(this->_curr_ptr->getData()); }
};
} // namespace HomeTask