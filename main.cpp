#include "headers/linked_list.h"
#include "headers/object.h"
#include <iostream>
#include <limits>
#include <random>
#include <ranges>

using namespace HomeTask;

std::unique_ptr<Task>
GetRandomTask(LinkedList<std::unique_ptr<Task>> &container) {
  static std::random_device randD;
  static std::mt19937 randMT(randD());

  std::uniform_int_distribution<int> taskRange(0, TaskTypeSize - 1);

  int16_t taskType = taskRange(randMT);

  switch (taskType) {
  case TaskType::BinTaskT: {

    std::uniform_int_distribution<int16_t> operatorRange(0, OpTypeSize - 1);

    const int MAX_OPERAND_VALUE = 100;
    const int MIN_OPERAND_VALUE = -100;

    std::uniform_int_distribution<int16_t> operandRange(MIN_OPERAND_VALUE,
                                                        MAX_OPERAND_VALUE);

    return std::make_unique<BinTask<int16_t>>(
        operandRange(randMT), operandRange(randMT),
        static_cast<OpType>(operatorRange(randMT)),
        NamedObject("really good name"));
  }
  case TaskType::AddTaskT: {
    auto addedTask = GetRandomTask(container);
    return std::make_unique<AddTask>(std::move(addedTask), container);
  }
  case TaskType::ObjectCountTaskT: {
    return std::make_unique<ObjectCountTask>(container);
  }
  case TaskType::ResultTasksCountTaskT: {
    return std::make_unique<ResultTasksCountTask>(container);
  }
  case TaskType::ClearContainerTaskT: {
    return std::make_unique<ClearContainerTask>(container);
  }
  case TaskType::AllExistingObjectsCountTaskT: {
    return std::make_unique<AllExistingObjectsCountTask>();
  }
  default:
    return nullptr;
  }
}

int main(int, char **) {
  LinkedList<std::unique_ptr<Task>> taskContainer;
  LinkedList<std::string> logContainer;

  for (size_t i = 0; i < 20; i++) {
    taskContainer.push_back(GetRandomTask(taskContainer));
  }

  std::cout << "Initial objects count: " << Object::GetAllExistingObjectsCount()
            << "\n\n";

  for (auto &it : taskContainer) {
    if (it->GetTaskType() == TaskType::ClearContainerTaskT) {
      it->Execute();
      break;
    }

    it->Execute();
    logContainer.push_back(it->ToString());
  }

  for (auto &log : logContainer) {
    std::cout << log;
  }

  std::cout << "\nObjects count after cleaning: "
            << Object::GetAllExistingObjectsCount();
}
