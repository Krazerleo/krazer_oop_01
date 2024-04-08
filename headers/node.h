#pragma once

template <typename T> class Node {
public:
  Node *next;
  Node *prev;

public:
  Node() : _data() {}
  explicit Node(const T &data) : _data(data) {}
  explicit Node(T &&data) : _data(std::move(data)) {}
  virtual ~Node() = default;

public:
  T &getData() { return _data; }
  T const &getData() const { return _data; }

  void setData(const T &data) { _data = data; }

  virtual void setData(T &&data) { _data = std::move(data); }

private:
  T _data;
};