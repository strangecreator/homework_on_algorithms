#include <algorithm>
#include <iostream>

namespace List {
// list implementation
template <typename T>
class List {
  // a sigle list el
  struct ListEl {
    T value;
    ListEl* previous;
    ListEl* next;

    ListEl(ListEl* previous = nullptr, ListEl* next = nullptr) {
      this->previous = previous;
      this->next = next;
    }

    ListEl(T value, ListEl* previous = nullptr, ListEl* next = nullptr) {
      this->value = value;
      this->previous = previous;
      this->next = next;
    }
  };

  ListEl* first_el_;
  ListEl* last_el_;
  size_t size_;

 public:
  List() {
    first_el_ = new ListEl();
    last_el_ = new ListEl(first_el_);
    first_el_->next = last_el_;
    size_ = 0;
  }

  ~List() {
    Clear();
    delete first_el_;
    delete last_el_;
  }

  void PushBack(T value) {
    ListEl* new_list_el = new ListEl(value, last_el_->previous, last_el_);
    last_el_->previous->next = new_list_el;
    last_el_->previous = new_list_el;
    size_++;
  }

  void PushFront(T value) {
    ListEl* new_list_el = new ListEl(value, first_el_, first_el_->next);
    first_el_->next->previous = new_list_el;
    first_el_->next = new_list_el;
    size_++;
  }

  void PopBack() {
    last_el_->previous = last_el_->previous->previous;
    delete last_el_->previous->next;
    last_el_->previous->next = last_el_;
    size_--;
  }

  void PopFront() {
    first_el_->next = first_el_->next->next;
    delete first_el_->next->previous;
    first_el_->next->previous = first_el_;
    size_--;
  }

  size_t Size() const { return size_; }

  T GetNth(size_t number) const {
    ListEl* element = first_el_;
    while (number-- != 0U) {
      element = element->next;
    }
    return element->value;
  }

  T GetFront() const { return GetNth(1); }

  T GetBack() const { return GetNth(Size()); }

  void Clear() {
    while (Size() > 0) {
      PopBack();
    }
  }
};
}  // namespace List

class GoblinQueueController {
  List::List<int> first_half_, second_half_;

 public:
  GoblinQueueController() {}

  void Balance() {
    while (first_half_.Size() > second_half_.Size() + 1) {
      second_half_.PushFront(first_half_.GetBack());
      first_half_.PopBack();
    }
    while (first_half_.Size() < second_half_.Size()) {
      first_half_.PushBack(second_half_.GetFront());
      second_half_.PopFront();
    }
  }

  void PushBack(int number, bool privileged = false) {
    if (privileged) {
      second_half_.PushFront(number);
    } else {
      second_half_.PushBack(number);
    }
    Balance();
  }

  int PopFront() {
    int number = first_half_.GetFront();
    first_half_.PopFront();
    Balance();
    return number;
  }
};

int main() {
  int amount_of_operations;
  int number;
  char operation;
  GoblinQueueController gqc;
  std::cin >> amount_of_operations;
  while ((amount_of_operations--) != 0) {
    std::cin >> operation;
    switch (operation) {
      case '+':
        std::cin >> number;
        gqc.PushBack(number);
        break;
      case '*':
        std::cin >> number;
        gqc.PushBack(number, true);
        break;
      case '-':
        std::cout << gqc.PopFront() << "\n";
    }
  }
}
