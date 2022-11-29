#include <algorithm>
#include <cmath>
#include <iostream>

struct Node {
  int value = -1;
  Node* next = nullptr;
  bool inchain = true;

  ~Node() {
    if (inchain) {
      delete next;
    }
  }

  void Push(int x) {
    Node* new_node = new Node{x, next};
    next = new_node;
  }

  void Remove(int x) {
    if (next == nullptr) {
      return;
    }
    if (next->value != x) {
      return next->Remove(x);
    }
    while (next != nullptr && next->value == x) {
      Node* new_next = next->next;
      next->inchain = false;
      delete next;
      next = new_next;
    }
  }

  bool In(int x) const {
    if (value == x) {
      return true;
    }
    if (next == nullptr) {
      return false;
    }
    return next->In(x);
  }
};

class HashTable {
  static const int kM = static_cast<int>(1e6);
  static const int kA = static_cast<int>(1e9 + 1137);
  static const int kB = 61;
  static const int kP = static_cast<int>(1e9 + 7);
  Node* nodes_;

  static int GetHash(int x) {
    return ((1130 * static_cast<long long>(x) + kB) % kP) % kM;
  }

 public:
  HashTable() : nodes_(new Node[kM]) {
    for (int i = 0; i < kM; ++i) {
      nodes_[i] = {-1, nullptr};
    }
  }

  ~HashTable() { delete[] nodes_; }

  void Add(int x) { nodes_[GetHash(x)].Push(x); }

  void Remove(int x) { nodes_[GetHash(x)].Remove(x); }

  bool In(int x) { return nodes_[GetHash(x)].In(x); }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, x;
  char cmd;
  std::cin >> n;
  HashTable hash_table;
  while (--n != -1) {
    std::cin >> cmd >> x;
    if (cmd == '+') {
      hash_table.Add(x);
    } else if (cmd == '-') {
      hash_table.Remove(x);
    } else {
      std::cout << (hash_table.In(x) ? "YES" : "NO") << std::endl;
    }
  }
}
