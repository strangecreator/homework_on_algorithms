#include <algorithm>
#include <iostream>
#include <string>

namespace Stack {
// single stack element implementation
class StackEl {
  int value_;
  int min_;
  size_t number_;
  StackEl* previous_;

 public:
  StackEl() : number_(0) {}

  StackEl(int value, StackEl* previous) {
    value_ = value;
    previous_ = previous;
    number_ = previous_->number_ + 1;
    if (previous_->number_ == 0) {
      min_ = value_;
    } else {
      min_ = std::min(previous_->min_, value_);
    }
  }

  int GetValue() const { return value_; }

  StackEl* GetPrevious() const { return previous_; }

  size_t GetNumber() const { return number_; }

  int GetMin() const { return min_; }
};

// stack implementation
class Stack {
  StackEl* last_el_;

 public:
  Stack() : last_el_(new StackEl()) {}

  ~Stack() {
    Clear();
    delete last_el_;
  }

  int Top() const { return last_el_->GetValue(); }

  void Pop() {
    StackEl* new_last_el = last_el_->GetPrevious();
    delete last_el_;
    last_el_ = new_last_el;
  }

  void Push(int value) { last_el_ = new StackEl(value, last_el_); }

  size_t Size() const { return last_el_->GetNumber(); }

  bool Empty() const { return Size() == 0; }

  int GetMin() const { return last_el_->GetMin(); }

  void Clear() {
    while (Size() > 0) {
      Pop();
    }
  }
};
}  // namespace Stack

namespace Queue {
class Queue {
  Stack::Stack left_stack_;
  Stack::Stack right_stack_;

  void Relocate() {
    while (!left_stack_.Empty()) {
      right_stack_.Push(left_stack_.Top());
      left_stack_.Pop();
    }
  }

 public:
  Queue() {}

  void Push(int value) { left_stack_.Push(value); }

  void Pop() {
    if (right_stack_.Empty()) {
      Relocate();
    }
    right_stack_.Pop();
  }

  int Back() { return left_stack_.Top(); }

  int Front() {
    if (right_stack_.Empty()) {
      Relocate();
    }
    return right_stack_.Top();
  }

  int GetMin() const {
    if (left_stack_.Empty()) {
      return right_stack_.GetMin();
    }
    if (right_stack_.Empty()) {
      return left_stack_.GetMin();
    }
    return std::min(left_stack_.GetMin(), right_stack_.GetMin());
  }

  size_t Size() const { return left_stack_.Size() + right_stack_.Size(); }

  void Clear() {
    left_stack_.Clear();
    right_stack_.Clear();
  }
};
}  // namespace Queue

class DistributingHat {
  Queue::Queue queue_;

 public:
  void PerformCmd(std::string cmd) {
    if (cmd == "dequeue" || cmd == "front" || cmd == "min") {
      if (queue_.Size() == 0) {
        std::cout << "error\n";
        return;
      }
    }
    if (cmd == "enqueue") {
      int iq_level;
      std::cin >> iq_level;
      queue_.Push(iq_level);
      std::cout << "ok\n";
    } else if (cmd == "dequeue") {
      std::cout << queue_.Front() << "\n";
      queue_.Pop();
    } else if (cmd == "front") {
      std::cout << queue_.Front() << "\n";
    } else if (cmd == "size") {
      std::cout << queue_.Size() << "\n";
    } else if (cmd == "clear") {
      queue_.Clear();
      std::cout << "ok\n";
    } else if (cmd == "min") {
      std::cout << queue_.GetMin() << "\n";
    }
  }
};

int main() {
  std::string cmd;
  int quantity;
  DistributingHat dist_hat;
  std::cin >> quantity;
  while (quantity-- != 0) {
    std::cin >> cmd;
    dist_hat.PerformCmd(cmd);
  }
}
