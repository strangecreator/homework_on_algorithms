#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

class Node {
  int value_ = -1;
  Node* next_ = nullptr;

 public:
  Node(int value, Node* next) : value_(value), next_(next) {}

  ~Node() { delete next_; }

  void Push(int x) {
    Node* new_node = new Node(x, next_);
    next_ = new_node;
  }

  void Remove(int x) {
    while (next_ != nullptr && next_->value_ == x) {
      Node* new_next = next_->next_;
      next_->next_ = nullptr;
      delete next_;
      next_ = new_next;
    }
    if (next_ == nullptr) {
      return;
    }
    return next_->Remove(x);
  }

  bool In(int x) const {
    if (value_ == x) {
      return true;
    }
    if (next_ == nullptr) {
      return false;
    }
    return next_->In(x);
  }
};

class HashTable {
  static const int kM = static_cast<int>(1e6);
  static const int kA = 1130;
  static const int kB = 61;
  static const int kP = static_cast<int>(1e9 + 7);
  std::vector<Node> nodes_;

  static int GetHash(int x) {
    return ((kA * static_cast<long long>(x) + kB) % kP) % kM;
  }

 public:
  HashTable() {
    nodes_.reserve(kM);
    for (int i = 0; i < kM; ++i) {
      nodes_.emplace_back(-1, nullptr);
    }
  }

  void Add(int x) { nodes_[GetHash(x)].Push(x); }

  void Remove(int x) { nodes_[GetHash(x)].Remove(x); }

  bool In(int x) const { return nodes_[GetHash(x)].In(x); }
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
