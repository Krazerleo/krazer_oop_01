#pragma once

#include "linked_list.h"
#include <atomic>
#include <cmath>
#include <memory>
#include <optional>
#include <string>

namespace HomeTask {

class Object {
public:
  static int64_t GetAllExistingObjectsCount() { return _objectCounter; }

  std::string virtual ToString() const noexcept = 0;

  Object([[maybe_unused]] Object const &object) { _objectCounter.fetch_add(1); }

  virtual ~Object() { _objectCounter.fetch_add(-1); };

protected:
  Object() { _objectCounter.fetch_add(1); };

private:
  inline static std::atomic_int64_t _objectCounter;
};

class NamedObject : public Object {
public:
  NamedObject(std::string const &name) : Object(), _name(name){};

  std::string ToString() const noexcept override { return _name; }

private:
  std::string _name;
};

enum TaskType {
  BinTaskT,
  AddTaskT,
  ObjectCountTaskT,
  ResultTasksCountTaskT,
  ClearContainerTaskT,
  AllExistingObjectsCountTaskT,
};

const size_t TaskTypeSize = 6;

class Task : public Object {
public:
  Task() : Object(){};

  void virtual Execute() = 0;
  TaskType virtual GetTaskType() const noexcept = 0;
  bool virtual CanHaveResult() const noexcept = 0;
};

enum OpType { Add, Subtract, Multiply, Divide };
const size_t OpTypeSize = 4;

template <typename T> class BinTask : public Task {
public:
  static_assert(std::is_integral<T>() || std::is_floating_point<T>());

  BinTask(T left, T right, OpType opType, NamedObject const &operationName)
      : _leftOperand(left), _rightOperand(right), _operationType(opType),
        _operationName(operationName){};

  void Execute() override {
    switch (_operationType) {
    case Add:
      _result = _leftOperand + _rightOperand;
      break;
    case Subtract:
      _result = _leftOperand - _rightOperand;
      break;
    case Multiply:
      _result = _leftOperand * _rightOperand;
      break;
    case Divide:
      if (std::abs(_rightOperand) > 0) {
        _result = _leftOperand / _rightOperand;
      }
      break;

    default:
      break;
    }
  }

  std::string ToString() const noexcept override {
    if (_result.has_value()) {
      return "Calculated value: " + std::to_string(_result.value()) + "\n";
    } else {
      return "Error occured";
    }
  }

  TaskType GetTaskType() const noexcept override { return TaskType::BinTaskT; }

  bool CanHaveResult() const noexcept override { return true; }

private:
  T _leftOperand;
  T _rightOperand;
  OpType _operationType;
  NamedObject _operationName;

  std::optional<T> _result;
};

class AddTask : public Task {
public:
  AddTask(std::unique_ptr<Task> task,
          LinkedList<std::unique_ptr<Task>> &container)
      : Task(), _taskToAdd(std::move(task)), _containerOwner(container){};

  void Execute() override {
    if (_taskToAdd != nullptr) {
      _containerOwner.push_back(std::move(_taskToAdd));
    }
  }

  std::string ToString() const noexcept override { return "Added new Task\n"; }

  TaskType GetTaskType() const noexcept override { return TaskType::AddTaskT; }

  bool CanHaveResult() const noexcept override { return false; }

private:
  std::unique_ptr<Task> _taskToAdd;
  LinkedList<std::unique_ptr<Task>> &_containerOwner;
};

class ObjectCountTask : public Task {
public:
  ObjectCountTask(LinkedList<std::unique_ptr<Task>> const &container)
      : Task(), _containerOwner(container){};

  void Execute() override { resultCount = _containerOwner.size(); }

  std::string ToString() const noexcept override {
    return "Total object in container:" + std::to_string(resultCount) + "\n";
  }

  TaskType GetTaskType() const noexcept override {
    return TaskType::ObjectCountTaskT;
  }

  bool CanHaveResult() const noexcept override { return true; }

private:
  LinkedList<std::unique_ptr<Task>> const &_containerOwner;
  size_t resultCount;
};

class ResultTasksCountTask : public Task {
public:
  ResultTasksCountTask(LinkedList<std::unique_ptr<Task>> const &container)
      : Task(), _containerOwner(container){};

  void Execute() override {
    size_t count = 0;
    for (auto &it : _containerOwner) {
      if (it->CanHaveResult() == true) {
        count++;
      }
    }

    resultCount = count;
  }

  std::string ToString() const noexcept override {
    return "Total tasks in container: " + std::to_string(resultCount) + "\n";
  }

  TaskType GetTaskType() const noexcept override {
    return TaskType::ResultTasksCountTaskT;
  }

  bool CanHaveResult() const noexcept override { return true; }

private:
  LinkedList<std::unique_ptr<Task>> const &_containerOwner;
  size_t resultCount;
};

class ClearContainerTask : public Task {
public:
  ClearContainerTask(LinkedList<std::unique_ptr<Task>> &container)
      : Task(), _containerOwner(container){};

  void Execute() override { _containerOwner.clear(); }

  std::string ToString() const noexcept override { return ""; }

  TaskType GetTaskType() const noexcept override {
    return TaskType::ClearContainerTaskT;
  }

  bool CanHaveResult() const noexcept override { return false; }

private:
  LinkedList<std::unique_ptr<Task>> &_containerOwner;
};

class AllExistingObjectsCountTask : public Task {
public:
  AllExistingObjectsCountTask() : Task(){};

  void Execute() override {
    resultCount = Object::GetAllExistingObjectsCount();
  }

  std::string ToString() const noexcept override {
    return "Total object: " + std::to_string(resultCount) + "\n";
  }

  TaskType GetTaskType() const noexcept override {
    return TaskType::AllExistingObjectsCountTaskT;
  }

  bool CanHaveResult() const noexcept override { return true; }

private:
  size_t resultCount;
};
} // namespace HomeTask