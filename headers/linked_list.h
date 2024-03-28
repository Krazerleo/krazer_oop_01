#pragma once

#include <cstdint>
#include <initializer_list>
#include <stdexcept>

#include "iterator.h"

namespace HomeTask {
template <class T> class LinkedList {
public:
public:
  using value_type = T;
  using size_type = std::uint32_t;
  using difference_type = std::ptrdiff_t;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;

  using iterator = Iterator<T>;
  using const_iterator = ConstIterator<T>;

private:
  size_type count;
  BaseNode<T> *head;
  BaseNode<T> *tail;

public:
  LinkedList<T>() : count(0), head(new BaseNode<T>()), tail(new BaseNode<T>()) {
    head->next = tail;
    tail->prev = head;
  };

  ~LinkedList() {
    clear();
    delete (head);
    delete (tail);
  }

public:
  LinkedList &operator=(LinkedList &&other) {
    if (&other == this)
      return (*this);

    if (count != 0)
      clear();

    this->count = other.count;
    other.count = 0;
    delete head;
    delete tail;
    head = other.head;
    tail = other.tail;
    other.head = nullptr;
    other.tail = nullptr;
    return *this;
  }

public:
  reference front() {
    if (count == 0) {
      throw std::runtime_error("List is empty");
    }

    return head->next->getData();
  }

  const_reference front() const {
    if (count == 0) {
      throw std::runtime_error("List is empty");
    }

    return *head->next->getData();
  }

  reference back() {
    if (count == 0) {
      throw std::runtime_error("List is empty");
    }

    return tail->prev->getData();
  }

  const_reference back() const {
    if (count == 0) {
      throw std::runtime_error("List is empty");
    }

    return tail->prev->getData();
  }

public:
  iterator begin() noexcept { return iterator(*head->next); }

  const_iterator begin() const noexcept { return const_iterator(*head->next); }

  const_iterator cbegin() const noexcept { return const_iterator(*head->next); }

  iterator end() noexcept { return iterator(*tail); }

  const_iterator end() const noexcept { return const_iterator(*tail); }

  const_iterator cend() const noexcept { return const_iterator(*tail); }

public:
  size_type size() const noexcept { return count; }

  bool empty() const noexcept { return 0 == count; }

public:
  iterator insert(const_iterator pos, const T &value) {
    if (pos._curr_ptr == cbegin()._curr_ptr->prev)
      throw std::runtime_error("Insert before the head");

    auto *_node = new BaseNode<T>(value);
    _node->prev = pos._curr_ptr->prev;
    _node->next = pos._curr_ptr;
    pos._curr_ptr->prev->next = _node;
    pos._curr_ptr->prev = _node;
    count++;
    return iterator(*_node);
  }

  iterator insert(const_iterator pos, T &&value) {
    if (pos._curr_ptr == cbegin()._curr_ptr->prev)
      throw std::runtime_error("Insert past the tail");

    auto *_node = new BaseNode<T>(std::move(value));
    _node->prev = pos._curr_ptr->prev;
    _node->next = pos._curr_ptr;
    pos._curr_ptr->prev->next = _node;
    pos._curr_ptr->prev = _node;
    count++;
    return iterator(*_node);
  }

  void push_front(const T &value) { insert(cbegin(), value); };

  void push_front(T &&value) { insert(cbegin(), std::move(value)); }

  void push_back(const T &value) { insert(cend(), value); }
  void push_back(T &&value) { insert(cend(), std::move(value)); }

public:
  iterator erase(const_iterator pos) {
    if (count == 0)
      throw std::runtime_error("List is empty");

    auto result = pos;
    ++result;
    pos._curr_ptr->prev->next = pos._curr_ptr->next;
    pos._curr_ptr->next->prev = pos._curr_ptr->prev;
    delete (pos._curr_ptr);
    count--;
    return iterator(*(result._curr_ptr));
  }

  void pop_back() { erase(--cend()); }
  void pop_front() { erase(cbegin()); }

public:
  void reverse() noexcept {
    if (count == 0 || count == 1)
      return;

    BaseNode<T> *cached_tail = head;
    BaseNode<T> *current = head;
    BaseNode<T> *temp = nullptr;

    while (current != nullptr) {
      temp = current->prev;
      current->prev = current->next;
      current->next = temp;
      current = current->prev;
    }

    head = temp->prev;
    tail = cached_tail;
  }

public:
  void clear() noexcept {
    if (count == 0)
      return;

    for (; count != 0;)
      pop_back();

    head->next = tail;
    tail->prev = head;
  }
};
} // namespace HomeTask