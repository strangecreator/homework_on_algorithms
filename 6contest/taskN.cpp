#include <iostream>
#include <algorithm>
#include <string>
#include <climits>

void Solve() {
    // speeding up
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    // initialization
    std::string a_str, b_str;
    int k;
    std::cin >> a_str >> b_str >> k;
    if (a_str.size() < b_str.size()) {
        std::swap(a_str, b_str);
    }
    int a_size = static_cast<int>(a_str.size()), b_size = static_cast<int>(b_str.size());
    if (a_size - b_size > k) {
        std::cout << -1;
        return;
    }
    int*** dp = new int**[b_size + 1];
    for (int i = 0; i <= b_size; ++i) {
        dp[i] = new int*[k + 1];
        for (int a = 0; a <= k; ++a) {
            dp[i][a] = new int[k + 1];
            for (int d = 0; d <= k; ++d) {
                int length = i + d - a;
                if (length > a_size || length < 0 || d + a > k) {
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
                    dp[i][a][d] = std::max({dp[i][a][d], dp[i - 1][a][d] + ((a_str[length - 1] == b_str[i - 1]) ? 1 : 0)});
                }
            }
        }
    }
    // finding the result
    int result = INT_MAX;
    for (int a = 0; a <= k; ++a) {
        int d = a_size + a - b_size;
        if (a + d > k) {
            break;
        }
        int new_result = std::max({0, b_size - dp[b_size][a][d] - (k - a - d)});
        result = std::min(result, new_result);
    }
    std::cout << result;
    // freeing up memory
    for (int i = 0; i <= b_size; ++i) {
        for (int a = 0; a <= k; ++a) {
            delete[] dp[i][a];
        }
        delete[] dp[i];
    }
    delete[] dp;
}

int main() {
    Solve();
}
