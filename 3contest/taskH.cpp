#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>

class SparseTable {
  struct TypeOfMin {
    int value, index;
  };

  using ElType = std::pair<TypeOfMin, TypeOfMin>;  // (first, second)

  int n_;
  int levels_;
  ElType** array_;

  static inline int GetIndex(int length);

  static inline int GetLength(int index) { return 1 << index; }

 public:
  SparseTable(int n);

  ~SparseTable();

  void Read();

  int FindSecond(int left, int right);

  friend bool operator<(const SparseTable::TypeOfMin& t1,
                        const SparseTable::TypeOfMin& t2);
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, m, left, right;
  std::cin >> n >> m;
  SparseTable sparse_table(n);
  sparse_table.Read();
  while (--m != -1) {
    std::cin >> left >> right;
    std::cout << sparse_table.FindSecond(left - 1, right - 1) << std::endl;
  }
}

bool operator<(const SparseTable::TypeOfMin& t1,
               const SparseTable::TypeOfMin& t2) {
  return (t1.value < t2.value) ||
         (t1.value == t2.value && t1.index == t2.index);
}

inline int SparseTable::GetIndex(int length) {
  return std::floor(std::log2(length));
}

SparseTable::SparseTable(int n)
    : n_(n), levels_(GetIndex(n_) + 1), array_(new ElType*[levels_]) {}

SparseTable::~SparseTable() {
  for (int i = 0; i < levels_; ++i) {
    delete[] array_[i];
  }
  delete[] array_;
}

void SparseTable::Read() {
  array_[0] = new ElType[n_];
  int number;
  for (int i = 0; i < n_; ++i) {
    std::cin >> number;
    array_[0][i] = {{number, i}, {INT_MAX, -1}};
  }
  for (int level = 2; level <= levels_; ++level) {
    int length = 1 << (level - 1);
    array_[level - 1] = new ElType[n_ - length + 1];
    for (int i = 0; i + length <= n_; ++i) {
      ElType left_half = array_[level - 2][i];
      ElType right_half = array_[level - 2][i + GetLength(level - 2)];
      array_[level - 1][i] = {
          std::min(left_half.first, right_half.first),
          std::min({std::max(left_half.first, right_half.first),
                    left_half.second, right_half.second})};
    }
  }
}

int SparseTable::FindSecond(int left, int right) {
  int index = GetIndex(right - left + 1);
  ElType& left_half = array_[index][left];
  ElType& right_half = array_[index][right - GetLength(index) + 1];
  if (left_half.first.index != right_half.first.index) {
    return std::min({std::max(left_half.first.value, right_half.first.value),
                     left_half.second.value, right_half.second.value});
  }
  return std::min(left_half.second.value, right_half.second.value);
}
