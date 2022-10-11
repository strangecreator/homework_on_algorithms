#include <cmath>
#include <iomanip>
#include <iostream>

int main() {
  std::ios::sync_with_stdio(false);
  double noise;
  int length;
  int q_requests;
  int left;
  int right;
  std::cin >> length;
  double* array = new double[length + 1];
  array[0] = 0;
  for (int i = 0; i < length; i++) {
    std::cin >> noise;
    array[i + 1] = array[i] + std::log(noise);
  }
  std::cin >> q_requests;
  while (q_requests-- != 0) {
    std::cin >> left >> right;
    double result =
        std::exp((array[right + 1] - array[left]) / (right - left + 1));
    std::cout << std::fixed << std::setprecision(10) << result << "\n";
  }
  delete[] array;
}
