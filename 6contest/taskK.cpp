#include <algorithm>
#include <iostream>

// a single cell in a dp array
struct DPCell {
  int cost = 0;
  bool in_use = false;
  int amount_selected = 0;
};

int main() {
  // speeding up
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  // initialization
  int n, m;
  std::cin >> n >> m;
  int* w = new int[n];
  int* c = new int[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> w[i];
  }
  for (int i = 0; i < n; ++i) {
    std::cin >> c[i];
  }
  DPCell** dp = new DPCell*[n + 1];
  for (int i = 0; i <= n; ++i) {
    dp[i] = new DPCell[m + 1];
    for (int j = 0; j <= m; ++j) {
      if (i == 0 || j == 0) {
        dp[i][j] = {0, false, 0};
        continue;
      }
      dp[i][j] = std::max(
          {DPCell{dp[i - 1][j].cost, false, dp[i - 1][j].amount_selected},
           (j >= w[i - 1])
               ? DPCell{dp[i - 1][j - w[i - 1]].cost + c[i - 1], true,
                        dp[i - 1][j - w[i - 1]].amount_selected + 1}
               : DPCell()},
          [](const DPCell& c1, const DPCell& c2) -> bool {
            return c1.cost < c2.cost;
          });
    }
  }
  // restoring the path
  int* indices = new int[dp[n][m].amount_selected];
  int i = n, j = m;
  for (int u = dp[n][m].amount_selected - 1; u >= 0; --u) {
    while (!dp[i][j].in_use) {
      --i;
    }
    indices[u] = i;
    j -= w[i - 1];
    --i;
  }
  for (int i = 0; i < dp[n][m].amount_selected; ++i) {
    std::cout << indices[i] << std::endl;
  }
  // freeing up memory
  delete[] w;
  delete[] c;
  for (int i = 0; i <= n; ++i) {
    delete[] dp[i];
  }
  delete[] dp;
  delete[] indices;
}
