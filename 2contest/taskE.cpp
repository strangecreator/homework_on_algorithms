#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

// implementation of binary heap with decrease key operation
class BinaryHeap {
  const int kLowerBound = -std::pow(10, 9) - 1;

  std::vector<std::pair<int, int>> heap_;
  std::vector<int> controller_;

  // updates node's pointer
  void UpdateNode(int index) { controller_[heap_[index].second] = index; }

  // checks for children
  bool HasChildren(int index) const {
    return index * 2 + 1 < static_cast<int>(heap_.size());
  }

  // checks for parent
  bool HasParent(int index) const { return index != 0; }

  // returns the index of the child with the lowest value
  int GetSmallestChild(int index) const;

  // sift the node up the tree
  void SiftUp(int child);

  // sift the node down the tree
  void SiftDown(int parent);

 public:
  void Insert(int number) {
    heap_.push_back({number, controller_.size()});
    controller_.push_back(heap_.size() - 1);
    SiftUp(heap_.size() - 1);
  }

  std::pair<int, int> GetMin() const { return heap_[0]; }

  void ExtractMin();

  void SetValueByKey(int key, int new_value) {
    heap_[controller_[key]].first = new_value;
    SiftUp(controller_[key]);
    SiftDown(controller_[key]);
  }

  void ExtractByKey(int key) {
    SetValueByKey(key, kLowerBound);
    SiftUp(controller_[key]);
    ExtractMin();
  }

  size_t Size() const { return heap_.size(); }

  void Clear() {
    for (int i = 0; i < static_cast<int>(Size()); ++i) {
      controller_[heap_[i].second] = -1;
    }
    heap_.clear();
  }
};

class MiniMaxHeap {
  BinaryHeap minHeap_, maxHeap_;

 public:
  void Insert(int number) {
    minHeap_.Insert(number);
    maxHeap_.Insert(-number);
  }

  size_t Size() const { return minHeap_.Size(); }

  int GetMin() const { return minHeap_.GetMin().first; }

  int GetMax() const { return -maxHeap_.GetMin().first; }

  void ExtractMin() {
    int key = minHeap_.GetMin().second;
    minHeap_.ExtractMin();
    maxHeap_.ExtractByKey(key);
  }

  void ExtractMax() {
    int key = maxHeap_.GetMin().second;
    maxHeap_.ExtractMin();
    minHeap_.ExtractByKey(key);
  }

  void Clear() {
    minHeap_.Clear();
    maxHeap_.Clear();
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cout.tie(nullptr);
  int m, number;
  std::cin >> m;
  std::string cmd;
  MiniMaxHeap heap;
  while (m-- != 0) {
    std::cin >> cmd;
    if (cmd == "insert") {
      std::cin >> number;
      heap.Insert(number);
      std::cout << "ok"
                << "\n";
    } else if (cmd == "clear") {
      heap.Clear();
      std::cout << "ok"
                << "\n";
    } else if (cmd == "size") {
      std::cout << heap.Size() << "\n";
    } else {
      if (heap.Size() == 0) {
        std::cout << "error"
                  << "\n";
      } else {
        if (cmd == "get_min") {
          std::cout << heap.GetMin() << "\n";
        } else if (cmd == "get_max") {
          std::cout << heap.GetMax() << "\n";
        } else if (cmd == "extract_min") {
          std::cout << heap.GetMin() << "\n";
          heap.ExtractMin();
        } else if (cmd == "extract_max") {
          std::cout << heap.GetMax() << "\n";
          heap.ExtractMax();
        }
      }
    }
  }
}

void BinaryHeap::SiftUp(int child) {
  while (HasParent(child)) {
    int parent = (child - 1) / 2;
    if (heap_[parent].first > heap_[child].first) {
      std::swap(heap_[parent], heap_[child]);
      UpdateNode(parent);
      UpdateNode(child);
      child = parent;
    } else {
      break;
    }
  }
}

void BinaryHeap::SiftDown(int parent) {
  while (HasChildren(parent)) {
    int child = GetSmallestChild(parent);
    if (heap_[child].first < heap_[parent].first) {
      std::swap(heap_[child], heap_[parent]);
      UpdateNode(child);
      UpdateNode(parent);
      parent = child;
    } else {
      break;
    }
  }
}

int BinaryHeap::GetSmallestChild(int index) const {
  if (index * 2 + 2 < static_cast<int>(heap_.size()) &&
      heap_[index * 2 + 1].first > heap_[index * 2 + 2].first) {
    return index * 2 + 2;
  }
  return index * 2 + 1;
}

void BinaryHeap::ExtractMin() {
  std::swap(heap_[0], heap_.back());
  controller_[heap_[heap_.size() - 1].second] = -1;
  heap_.pop_back();
  if (Size() > 0) {
    UpdateNode(0);
    SiftDown(0);
  }
}
