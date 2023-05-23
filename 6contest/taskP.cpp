#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>


class Solver {
  static const int kMod = 1'000'000'007;

  int n_, m_;
  char* array_;

  static int GetBit(int n, int i) {  // n is a mask
    return (n >> i) & 1;
  }

  static int InvertBits(int mask, int n) {  // 0 <= n
    return (1 << n) - mask - 1;
  }

  static bool CheckMask(int mask, int n) {
    for (int i = 1; i < n; ++i) {
      // check 3 bits
      if (GetBit(mask, i - 1) == GetBit(mask, i)) {
        return false;
      }
    }
    return true;
  }

  int** CalculateDynamic() const {
    // mask dp
    int** dp = new int*[m_];
    for (int i = 0; i < m_; ++i) {
      dp[i] = new int[1 << n_];
      for (int j = 0; j < (1 << n_); ++j) {  // j is a mask
        // check a mask
        bool check = true;
        for (int u = 0; u < n_; ++u) {
          if (array_[u * m_ + i] != '.' &&
              (array_[u * m_ + i] == '+' ? 1 : 0) != GetBit(j, u)) {
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
        dp[i][j] = dp[i - 1][InvertBits(j, n_)];
        if (CheckMask(j, n_)) {
          dp[i][j] += dp[i - 1][j];
        }
        dp[i][j] %= kMod;
      }
    }
    return dp;
  }

  void FreeMemory(int** dp) const {
    for (int i = 0; i < m_; ++i) {
      delete[] dp[i];
    }
    delete[] dp;
  }

 public:
  Solver(int n, int m) : n_(n), m_(m),
  array_(new char[n * m]) {}

  ~Solver() {
    delete[] array_;
  }

  void Read(std::istream& stream) {
    for (int i = 0; i < n_; ++i) {
      for (int j = 0; j < m_; ++j) {
        stream >> array_[i * m_ + j];
      }
    }
  }

  int FindResult() {
    int** dp = CalculateDynamic();
    int result = 0;
    for (int i = 0; i < (1 << n_); ++i) {
      result = (result + dp[m_ - 1][i]) % kMod;
    }
    // freeing up memory
    FreeMemory(dp);
    return result;
  }
};

int main() {
  // speeding up
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  // initialization
  int n, m;
  std::cin >> n >> m;
  Solver solver(n, m);
  solver.Read(std::cin);
  // finding the result
  std::cout << solver.FindResult();
}
