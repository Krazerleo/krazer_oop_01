#pragma once

template <typename T> class BaseNode {
public:
  BaseNode *next;
  BaseNode *prev;

public:
  BaseNode() : _data() {}
  explicit BaseNode(const T &data) : _data(data) {}
  explicit BaseNode(T &&data) : _data(std::move(data)) {}
  virtual ~BaseNode() = default;

public:
  T &getData() { return _data; }

  void setData(const T &data) { _data = data; }

  virtual void setData(T &&data) { _data = std::move(data); }

private:
  T _data;
};