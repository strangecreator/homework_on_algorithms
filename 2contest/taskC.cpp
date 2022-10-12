#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// implementation of binary heap with decrease key operation
class BinaryHeap {
  std::vector<std::pair<long long, int>> heap_;
  std::vector<std::pair<int, int>> controller_;

  // finds controller's node by key
  int FindByKey(int key) const {
    // binary search implementation
    int left = 0;
    int right = controller_.size() - 1;
    while (right - left != 0) {
      int middle = (right + left) / 2;
      if (controller_[middle].first < key) {
        left = middle + 1;
      } else if (controller_[middle].first > key) {
        right = middle - 1;
      } else {
        return middle;
      }
    }
    return left;
  }

  // updates node's pointer
  void UpdateNode(int index) {
    controller_[heap_[index].second].second = index;
  }

  // checks for children
  bool HasChildren(int index) const {
    return index * 2 + 1 < (int)heap_.size();
  }

  // checks for parent
  bool HasParent(int index) const { return index != 0; }

  // returns the index of the child with the lowest value
  int GetSmallestChild(int index) {
    if (index * 2 + 2 < (int)heap_.size() &&
        heap_[index * 2 + 1].first > heap_[index * 2 + 2].first) {
      return index * 2 + 2;
    }
    return index * 2 + 1;
  }

  // sift the node up the tree
  void SiftUp(int child) {
    while (HasParent(child)) {
      int parent = (child - 1) / 2;
      if (heap_[parent] > heap_[child]) {
        std::swap(heap_[parent], heap_[child]);
        UpdateNode(parent);
        UpdateNode(child);
        child = parent;
      } else {
        break;
      }
    }
  }

  // sift the node down the tree
  void SiftDown(int parent) {
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

 public:
  void Insert(int number, int key) {
    heap_.push_back({number, controller_.size()});
    controller_.push_back({key, heap_.size() - 1});
    SiftUp(heap_.size() - 1);
  }

  long long GetMin() const { return heap_[0].first; }

  void ExtractMin() {
    std::swap(heap_[0], heap_.back());
    UpdateNode(0);
    controller_[heap_[heap_.size() - 1].second].second = -1;
    heap_.pop_back();
    SiftDown(0);
  }

  void DecreaseKey(int key, int delta) {
    int node = FindByKey(key);
    heap_[controller_[node].second].first -= delta;
    SiftUp(controller_[node].second);
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cout.tie(nullptr);
  int q, number, key, delta;
  std::string cmd;
  BinaryHeap heap;
  std::cin >> q;
  for (int i = 1; i <= q; i++) {
    std::cin >> cmd;
    if (cmd == "insert") {
      std::cin >> number;
      heap.Insert(number, i);
    } else if (cmd == "getMin") {
      std::cout << heap.GetMin() << "\n";
    } else if (cmd == "extractMin") {
      heap.ExtractMin();
    } else if (cmd == "decreaseKey") {
      std::cin >> key >> delta;
      heap.DecreaseKey(key, delta);
    }
  }
}
