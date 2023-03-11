#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>

class LastOccurenceInPrefix {
  int n_;
  int* array_;
  int* quantities_;
  int* search_array_;

 public:
  LastOccurenceInPrefix(int* array, int n)
      : n_(n),
        array_(array),
        quantities_(new int[n + 1]),
        search_array_(new int[n]) {
    for (int i = 0; i <= n; ++i) {
      quantities_[i] = 0;
    }
    for (int i = 0; i < n; ++i) {
      ++quantities_[array[i] + 1];
    }
    for (int i = 1; i <= n; ++i) {
      quantities_[i] += quantities_[i - 1];
    }
    int* current_indices = new int[n + 1];
    for (int i = 0; i <= n; ++i) {
      current_indices[i] = quantities_[i];
    }
    for (int i = 0; i < n; ++i) {
      search_array_[current_indices[array[i]]] = i;
      ++current_indices[array[i]];
    }
    delete[] current_indices;
  }

  ~LastOccurenceInPrefix() {
    delete[] quantities_;
    delete[] search_array_;
  }

  int Get(int x, int i) const {
    return *(std::upper_bound(search_array_ + quantities_[x],
                              search_array_ + quantities_[x + 1], i) -
             1);
  }
};

int main() {
  // speeding up
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  // initialization and finding the result length
  int n;
  std::cin >> n;
  int64_t* array = new int64_t[n];
  int64_t* dp_array = new int64_t[n];
  int* modified_indices = new int[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> array[i];
    dp_array[i] = LLONG_MIN;
    int index = std::distance(
        dp_array,
        std::upper_bound(dp_array, dp_array + i + 1, array[i],
                         [](const int64_t& x, const int64_t& y) -> bool {
                           return -x < -y;
                         }));
    dp_array[index] = array[i];
    modified_indices[i] = index;
  }
  int length = *std::max_element(modified_indices, modified_indices + n) + 1;
  std::cout << length << std::endl;
  // restoring the path
  LastOccurenceInPrefix loip(modified_indices, n);
  int* indices = new int[length + 1];
  indices[length] = n;
  for (int i = length; i > 0; --i) {
    indices[i - 1] = loip.Get(i - 1, indices[i] - 1);
  }
  for (int i = 0; i < length; ++i) {
    std::cout << indices[i] + 1 << " ";
  }
  // freeing up memory
  delete[] array;
  delete[] dp_array;
  delete[] modified_indices;
  delete[] indices;
}
