#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>

class SegmentsTree {
  int n_;
  int levels_;
  int size_;
  int* array_;

  static inline int GetParent(int i) {
    if (i == 0) {
      return -1;
    }
    return (i - 1) >> 1;
  }

  inline int GetLeftChild(int i) const {
    return ((i << 1) + 1 < size_) ? ((i << 1) + 1) : -1;
  }

  inline int GetRightChild(int i) const {
    return ((i << 1) + 2 < size_) ? ((i << 1) + 2) : -1;
  }

  inline long long GetValue(int i) const {
    if (-1 < i && i < size_) {
      return array_[i];
    }
    return INT_MAX;
  }

  std::pair<int, int> GetSegment(int i) const {
    int level = std::floor(std::log2(i + 1)) + 1;
    int left = ((i + 1) << (levels_ - level)) - (1 << (levels_ - 1));
    int right = left + (1 << (levels_ - level)) - 1;
    return {left, right};
  }

  int GetNodeThatGreater(int i, int left, int right, int x) const {
    std::pair<int, int> segment = GetSegment(i);
    if (segment.first == left && segment.second == right) {
      return (GetValue(i) < x) ? -1 : i;
    }
    int middle = (segment.first + segment.second) >> 1;
    if (left <= middle) {
      int new_right = std::min(middle, right);
      int node = GetNodeThatGreater(GetLeftChild(i), left, new_right, x);
      if (node != -1) {
        return node;
      }
    }
    if (middle < right) {
      int new_left = std::max(left, middle + 1);
      int node = GetNodeThatGreater(GetRightChild(i), new_left, right, x);
      if (node != -1) {
        return node;
      }
    }
    return -1;
  }

 public:
  SegmentsTree(int n)
      : n_(n),
        levels_(std::ceil(std::log2(n_)) + 1),
        size_((1 << (levels_ - 1)) + n_ - 1),
        array_(new int[size_]) {}

  ~SegmentsTree() { delete[] array_; }

  void Read(std::istream& stream) {
    for (int i = (1 << (levels_ - 1)) - 1; i < size_; ++i) {
      stream >> array_[i];
    }
    for (int i = levels_ - 2; i >= 0; --i) {
      for (int j = (1 << i) - 1; j < (1 << (i + 1)) - 1; ++j) {
        array_[j] =
            std::max(GetValue(GetLeftChild(j)), GetValue(GetRightChild(j)));
      }
    }
  }

  void Assign(int i, int new_value) {
    i = (1 << (levels_ - 1)) + (i - 2);
    while (i != -1) {
      int left = GetLeftChild(i);
      int right = GetRightChild(i);
      if (left == -1) {
        array_[i] = new_value;
      } else {
        array_[i] = std::max(GetValue(left), GetValue(right));
      }
      i = GetParent(i);
    }
  }

  int GetGreater(int i, int x) {
    int node = GetNodeThatGreater(0, i - 1, n_ - 1, x);
    if (node == -1) {
      return -1;
    }
    while (GetLeftChild(node) != -1) {
      if (x <= GetValue(GetLeftChild(node))) {
        node = GetLeftChild(node);
      } else {
        node = GetRightChild(node);
      }
    }
    return node - (1 << (levels_ - 1)) + 2;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, m, cmd, i, x;
  std::cin >> n >> m;
  SegmentsTree segments_tree(n);
  segments_tree.Read(std::cin);
  while (--m != -1) {
    std::cin >> cmd >> i >> x;
    if (cmd == 0) {
      segments_tree.Assign(i, x);
    } else {
      std::cout << segments_tree.GetGreater(i, x) << "\n";
    }
  }
}
