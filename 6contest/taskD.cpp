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
  int** dp = new int*[k];
  for (int j = 0; j < k; ++j) {
    dp[j] = new int[n];
    for (int i = 0, m = 0; i < n; ++i) {
      if (i == 0) {
        dp[j][i] = 0;
        continue;
      }
      if (j == 0) {
        dp[j][i] = i;
        continue;
      }
      while (m < i - 1 && std::max({dp[j - 1][m], dp[j][i - m - 1]}) ==
                              std::max({dp[j - 1][m + 1], dp[j][i - m - 2]})) {
        ++m;
      }
      dp[j][i] = 1 + dp[j - 1][m];
    }
  }
  std::cout << dp[k - 1][n - 1];
  // freeing up memory
  for (int j = 0; j < k; ++j) {
    delete[] dp[j];
  }
  delete[] dp;
}
