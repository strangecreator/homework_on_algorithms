#include <algorithm>
#include <iostream>

// a single cell in a dp array
struct DPCell {
  int cost = 0;
  bool in_use = false;
  int amount_selected = 0;
};

class Backpack {
  int n_, m_;
  int* w_;  // weight
  int* c_;  // cost
  DPCell** dp_;

 public:
  Backpack(int n, int m)
      : n_(n), m_(m), w_(new int[n]), c_(new int[n]), dp_(new DPCell*[n + 1]) {}

  ~Backpack() {
    delete[] w_;
    delete[] c_;
    for (int i = 0; i <= n_; ++i) {
      delete[] dp_[i];
    }
    delete[] dp_;
  }

  void Read(std::istream& stream) {
    for (int i = 0; i < n_; ++i) {
      stream >> w_[i];
    }
    for (int i = 0; i < n_; ++i) {
      stream >> c_[i];
    }
  }

  int Solve() {
    for (int i = 0; i <= n_; ++i) {
      dp_[i] = new DPCell[m_ + 1];
      for (int j = 0; j <= m_; ++j) {
        if (i == 0 || j == 0) {
          dp_[i][j] = {0, false, 0};
          continue;
        }
        dp_[i][j] = std::max(
            {DPCell{dp_[i - 1][j].cost, false, dp_[i - 1][j].amount_selected},
             (j >= w_[i - 1])
                 ? DPCell{dp_[i - 1][j - w_[i - 1]].cost + c_[i - 1], true,
                          dp_[i - 1][j - w_[i - 1]].amount_selected + 1}
                 : DPCell()},
            [](const DPCell& c1, const DPCell& c2) -> bool {
              return c1.cost < c2.cost;
            });
      }
    }
    return dp_[n_][m_].amount_selected;
  }

  void FindPath(int* indices) const {
    int i = n_, j = m_;
    for (int u = dp_[n_][m_].amount_selected - 1; u >= 0; --u) {
      while (!dp_[i][j].in_use) {
        --i;
      }
      indices[u] = i;
      j -= w_[i - 1];
      --i;
    }
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
  Backpack backpack(n, m);
  backpack.Read(std::cin);
  int result = backpack.Solve();
  // restoring the path
  int* indices = new int[result];
  backpack.FindPath(indices);
  for (int i = 0; i < result; ++i) {
    std::cout << indices[i] << std::endl;
  }
  // freeing up memory
  delete[] indices;
}
