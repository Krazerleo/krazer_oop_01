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
  Node<T> *head;
  Node<T> *tail;

public:
  LinkedList<T>() : count(0), head(new Node<T>()), tail(new Node<T>()) {
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

    if (empty() == false)
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
    if (empty()) {
      throw std::runtime_error("List is empty");
    }

    return head->next->getData();
  }

  const_reference front() const {
    if (empty()) {
      throw std::runtime_error("List is empty");
    }

    return *head->next->getData();
  }

  reference back() {
    if (empty()) {
      throw std::runtime_error("List is empty");
    }

    return tail->prev->getData();
  }

  const_reference back() const {
    if (empty()) {
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
    if (pos._ptr == cbegin()._ptr->prev)
      throw std::runtime_error("Insert before the head");

    auto *_node = new Node<T>(value);
    _node->prev = pos._ptr->prev;
    _node->next = pos._ptr;
    pos._ptr->prev->next = _node;
    pos._ptr->prev = _node;
    count++;
    return iterator(*_node);
  }

  iterator insert(const_iterator pos, T &&value) {
    if (pos._ptr == cbegin()._ptr->prev)
      throw std::runtime_error("Insert past the tail");

    auto *_node = new Node<T>(std::move(value));
    _node->prev = pos._ptr->prev;
    _node->next = pos._ptr;
    pos._ptr->prev->next = _node;
    pos._ptr->prev = _node;
    count++;
    return iterator(*_node);
  }

  void push_front(const T &value) { insert(cbegin(), value); };

  void push_front(T &&value) { insert(cbegin(), std::move(value)); }

  void push_back(const T &value) { insert(cend(), value); }
  void push_back(T &&value) { insert(cend(), std::move(value)); }

public:
  iterator erase(const_iterator pos) {
    if (empty())
      throw std::runtime_error("List is empty");

    auto result = pos;
    ++result;
    pos._ptr->prev->next = pos._ptr->next;
    pos._ptr->next->prev = pos._ptr->prev;
    delete (pos._ptr);
    count--;
    return iterator(*(result._ptr));
  }

  void pop_back() { erase(--cend()); }
  void pop_front() { erase(cbegin()); }

public:
  void reverse() noexcept {
    if (count == 0 || count == 1)
      return;

    Node<T> *cached_tail = head;
    Node<T> *current = head;
    Node<T> *temp = nullptr;

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
    if (empty())
      return;

    while (empty() == false) {
      pop_back();
    }

    head->next = tail;
    tail->prev = head;
  }
};
} // namespace HomeTask