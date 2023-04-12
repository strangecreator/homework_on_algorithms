#include <algorithm>
#include <cmath>
#include <iostream>

// if there are enough planes we can just build binary search
int Limit(int n) { return std::ceil(std::log2(n)); }

int main() {
  // initialization
  int n, k;
  std::cin >> n >> k;
  if (k >= Limit(n)) {
    std::cout << Limit(n);
    return 0;
  }
  if (k == 0) {
    std::cout << -1;
    return 0;
  }
  // dp[j][i] - result of the subtask with j planes and height n
  int* layer1 = new int[n];
  int* layer2 = new int[n];
  for (int j = 0; j < k; ++j) {
    for (int i = 0, m = 0; i < n; ++i) {
      if (i == 0) {
        layer2[i] = 0;
        continue;
      }
      if (j == 0) {
        layer2[i] = i;
        continue;
      }
      while (m < i - 1 && std::max({layer1[m], layer2[i - m - 1]}) ==
                              std::max({layer1[m + 1], layer2[i - m - 2]})) {
        ++m;
      }
      layer2[i] = 1 + layer1[m];
    }
    // copy
    for (int i = 0; i < n; ++i) {
      layer1[i] = layer2[i];
    }
  }
  std::cout << layer2[n - 1];
  // freeing up memory
  delete[] layer1;
  delete[] layer2;
}
