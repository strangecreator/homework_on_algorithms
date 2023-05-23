#include <iostream>
#include <algorithm>
#include <string>
#include <climits>


class Solver {
    int k_;
    std::string a_str_, b_str_;

    int*** CalculateDynamic() {
        int a_size = static_cast<int>(a_str_.size()), b_size = static_cast<int>(b_str_.size());
        int*** dp = new int**[b_size + 1];
        for (int i = 0; i <= b_size; ++i) {
            dp[i] = new int*[k_ + 1];
            for (int a = 0; a <= k_; ++a) {
                dp[i][a] = new int[k_ + 1];
                for (int d = 0; d <= k_; ++d) {
                    int length = i + d - a;
                    if (length > a_size || length < 0 || d + a > k_) {
                        break;
                    }
                    dp[i][a][d] = 0;
                    if (i == 0) {
                        continue;
                    }
                    if (a > 0) {
                        dp[i][a][d] = std::max(dp[i][a][d], dp[i - 1][a - 1][d] + 1);
                    }
                    if (d > 0 && length > 0) {
                        dp[i][a][d] = std::max(dp[i][a][d], dp[i][a][d - 1]);
                    }
                    if (length > 0) {
                        dp[i][a][d] = std::max({dp[i][a][d], dp[i - 1][a][d] + ((a_str_[length - 1] == b_str_[i - 1]) ? 1 : 0)});
                    }
                }
            }
        }
        return dp;
    }

    void FreeMemory(int*** dp) {
        int a_size = static_cast<int>(a_str_.size()), b_size = static_cast<int>(b_str_.size());
        // freeing up memory
        for (int i = 0; i <= b_size; ++i) {
            for (int a = 0; a <= k_; ++a) {
                delete[] dp[i][a];
            }
            delete[] dp[i];
        }
        delete[] dp;
    }

    int FindResult(int*** dp) {
        int a_size = static_cast<int>(a_str_.size()), b_size = static_cast<int>(b_str_.size());
        int result = INT_MAX;
        for (int a = 0; a <= k_; ++a) {
            int d = a_size + a - b_size;
            if (a + d > k_) {
                break;
            }
            int new_result = std::max({0, b_size - dp[b_size][a][d] - (k_ - a - d)});
            result = std::min(result, new_result);
        }
        return result;
    }

  public:
    void Read(std::istream& stream) {
        stream >> a_str_ >> b_str_ >> k_;
        if (a_str_.size() < b_str_.size()) {
            std::swap(a_str_, b_str_);
        }
    }

    int Solve() {
        int a_size = static_cast<int>(a_str_.size()), b_size = static_cast<int>(b_str_.size());
        if (a_size - b_size > k_) {
            return -1;
        }
        int*** dp = CalculateDynamic();
        // find the result
        int result = FindResult(dp);
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
    Solver solver;
    solver.Read(std::cin);
    std::cout << solver.Solve();
}
