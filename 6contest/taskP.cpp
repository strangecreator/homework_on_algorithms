#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

int GetBit(int n, int i) {  // n is a mask
  return (n >> i) & 1;
}

int InvertBits(int mask, int n) {  // 0 <= n
  return (1 << n) - mask - 1;
}

bool CheckMask(int mask, int n) {
  for (int i = 1; i < n; ++i) {
    // check 3 bits
    if (GetBit(mask, i - 1) == GetBit(mask, i)) {
      return false;
    }
  }
  return true;
}

int main() {
  // speeding up
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  // initialization
  const int kMod = 1'000'000'007;
  int n, m;
  std::cin >> n >> m;
  char* array = new char[n * m];
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      std::cin >> array[i * m + j];
    }
  }
  int** dp = new int*[m];
  for (int i = 0; i < m; ++i) {
    dp[i] = new int[1 << n];
    for (int j = 0; j < (1 << n); ++j) {  // j is a mask
      // check a mask
      bool check = true;
      for (int u = 0; u < n; ++u) {
        if (array[u * m + i] != '.' &&
            (array[u * m + i] == '+' ? 1 : 0) != GetBit(j, u)) {
          check = false;
          break;
        }
      }
      if (!check) {
        dp[i][j] = 0;
        continue;
      }
      if (i == 0) {
        dp[i][j] = 1;
        continue;
      }
      // building a previous mask
      dp[i][j] = dp[i - 1][InvertBits(j, n)];
      if (CheckMask(j, n)) {
        dp[i][j] += dp[i - 1][j];
      }
      dp[i][j] %= kMod;
    }
  }
  // printing the result
  int result = 0;
  for (int i = 0; i < (1 << n); ++i) {
    result = (result + dp[m - 1][i]) % kMod;
  }
  std::cout << result << std::endl;
  // freeing up memory
  delete[] array;
  for (int i = 0; i < m; ++i) {
    delete[] dp[i];
  }
  delete[] dp;
}
