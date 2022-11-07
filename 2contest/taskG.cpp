#include <cmath>
#include <iostream>
#include <utility>

const int kChunks = 8;             // amount of chunks
const int kWeight = 64 / kChunks;  // length in bits
const int kLength = 1 << kWeight;  // amount of values

int GetRadix(uint64_t number, int i) {
  return (number >> (kWeight * i)) & (kLength - 1);
}

void LSDSort(uint64_t* array, int size) {
  int radix_amount[kLength];
  int indices[kLength + 1];
  uint64_t* array_trash = new uint64_t[size];
  for (int i = 0; i < kChunks; ++i) {
    for (int j = 0; j < size; ++j) {
      array_trash[j] = array[j];
    }
    for (int j = 0; j < kLength; ++j) {
      radix_amount[j] = 0;
    }
    for (int j = 0; j < size; ++j) {
      ++radix_amount[GetRadix(array[j], i)];
    }
    indices[0] = 0;
    for (int j = 1; j < kLength + 1; ++j) {
      indices[j] = indices[j - 1] + radix_amount[j - 1];
    }
    for (int j = 0; j < size; ++j) {
      int radix = GetRadix(array_trash[j], i);
      array[indices[radix]] = array_trash[j];
      ++indices[radix];
    }
  }
  delete[] array_trash;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n;
  std::cin >> n;
  uint64_t* array = new uint64_t[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> array[i];
  }
  LSDSort(array, n);
  for (int i = 0; i < n; ++i) {
    std::cout << array[i] << "\n";
  }
  delete[] array;
}
