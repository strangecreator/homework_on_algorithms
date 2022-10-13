#include <cmath>
#include <iostream>
#include <utility>

int Devide(int n, int k) {
  if (n % k == 0) {
    return n / k;
  }
  return n / k + 1;
}

// sorts an array that contains at most five elements,
// and returns a median
int Sort5(int* array, int left, int right) {
  // bubble sort
  for (int i = left; i < right; i++) {
    for (int j = i + 1; j <= right; j++) {
      if (array[i] > array[j]) {
        std::swap(array[i], array[j]);
      }
    }
  }
  // returns a median
  return left + Devide(right - left + 1, 2) - 1;
}

// sorts an array relative to a certain element,
// and returns an index to this element after all
int Partition(int* array, int left, int right, int pivot_index) {
  // finds elements that less than or equal to pivot
  std::swap(array[left], array[pivot_index]);
  int j = left + 1;
  for (int i = left + 1; i <= right; i++) {
    if (array[i] <= array[left]) {
      std::swap(array[j++], array[i]);
    }
  }
  std::swap(array[left], array[j - 1]);
  // finds elements that less than pivot
  int u = left;
  for (int i = left; i < j; i++) {
    if (array[i] < array[j - 1]) {
      std::swap(array[u], array[i]);
      u++;
    }
  }
  int middle = left + Devide(right - left + 1, 2) - 1;
  if (u <= middle && middle <= j - 1) {
    return middle;
  }
  if (middle < u) {
    return u;
  }
  return j - 1;
}

int MedianOfMedians(int* array, int left, int right);

// returns an index to a kth smallest element (array will be modified!)
int Select(int* array, int left, int right, int k) {
  while (right - left >= 5) {
    // finds an approximate median index in [left, right] segment
    int median_index = MedianOfMedians(array, left, right);
    // sorts [left, right] segment relatively to an array[medianIndex]
    // and returns an index to this element
    int pivot_index = Partition(array, left, right, median_index);
    if (pivot_index + 1 == left + k) {
      return pivot_index;
    }
    if (pivot_index + 1 < left + k) {
      k -= (pivot_index + 1 - left);
      left = pivot_index + 1;
    } else {
      right = pivot_index - 1;
    }
  }
  Sort5(array, left, right);
  return left + k - 1;
}

// returns an approximate median of an array (array will be modified!)
int MedianOfMedians(int* array, int left, int right) {
  for (int i = left; i <= right; i += 5) {
    int sub_right = (i + 4 < right) ? i + 4 : right;
    int median_index = Sort5(array, i, sub_right);
    std::swap(array[median_index], array[left + (i - left) / 5]);
  }
  int new_right = left + Devide(right - left + 1, 5) - 1;
  int middle_position = Devide(new_right - left + 1, 2);
  return Select(array, left, new_right, middle_position);
}

void QuickSort(int* array, int left, int right) {
  if (right - left >= 5) {
    int median_index = MedianOfMedians(array, left, right);
    int pivot_index = Partition(array, left, right, median_index);
    QuickSort(array, left, pivot_index);
    if (pivot_index < right) {
      QuickSort(array, pivot_index + 1, right);
    }
  } else {
    Sort5(array, left, right);
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cout.tie(nullptr);
  int n;
  std::cin >> n;
  int* array = new int[n];
  for (int i = 0; i < n; i++) {
    std::cin >> array[i];
  }
  QuickSort(array, 0, n - 1);
  for (int i = 0; i < n; i++) {
    std::cout << array[i] << " ";
  }
  delete[] array;
}
