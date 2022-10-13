#include <cmath>
#include <iostream>
#include <utility>

int GetRadix(uint64_t number, int i) {
  return (number / (uint64_t)std::pow(256, i)) % 256;
}

void LSDSort(uint64_t* array, int size) {
  int radix_amount[256];
  int indices[257];
  uint64_t* array_trash = new uint64_t[size];
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < size; j++) {
      array_trash[j] = array[j];
    }
    for (int j = 0; j < 256; j++) {
      radix_amount[j] = 0;
    }
    for (int j = 0; j < size; j++) {
      ++radix_amount[GetRadix(array[j], i)];
    }
    indices[0] = 0;
    for (int j = 1; j < 257; j++) {
      indices[j] = indices[j - 1] + radix_amount[j - 1];
    }
    for (int j = 0; j < size; j++) {
      int radix = GetRadix(array_trash[j], i);
      array[indices[radix]] = array_trash[j];
      ++indices[radix];
    }
  }
  delete[] array_trash;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cout.tie(nullptr);
  int n;
  std::cin >> n;
  uint64_t* array = new uint64_t[n];
  for (int i = 0; i < n; i++) {
    std::cin >> array[i];
  }
  LSDSort(array, n);
  for (int i = 0; i < n; i++) {
    std::cout << array[i] << "\n";
  }
  delete[] array;
}
