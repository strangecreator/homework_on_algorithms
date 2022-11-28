#include <cmath>
#include <iostream>
#include <thread>
#include <utility>

static const int kMod = std::pow(10, 7) + 4321;
static const int kMinAmount = 5;

int CeilDivide(int n, int k) {
  if (n % k == 0) {
    return n / k;
  }
  return n / k + 1;
}

// sorts an array that contains at most five elements,
// and returns a median
int SortAndGetMedian(int* array, int left, int right) {
  // bubble sort
  for (int i = left; i < right; ++i) {
    for (int j = i + 1; j <= right; ++j) {
      if (array[i] > array[j]) {
        std::swap(array[i], array[j]);
      }
    }
  }
  // returns a median
  return left + CeilDivide(right - left + 1, 2) - 1;
}

// sorts an array relative to a certain element,
// and returns an index to this element after all
int Partition(int* array, int left, int right, int pivot_index) {
  // finds elements that less than or equal to pivot
  std::swap(array[left], array[pivot_index]);
  int j = left + 1;
  for (int i = left + 1; i <= right; ++i) {
    if (array[i] <= array[left]) {
      std::swap(array[j++], array[i]);
    }
  }
  std::swap(array[left], array[j - 1]);
  // finds elements that less than pivot
  int u = left;
  for (int i = left; i < j; ++i) {
    if (array[i] < array[j - 1]) {
      std::swap(array[u], array[i]);
      ++u;
    }
  }
  int middle = left + CeilDivide(right - left + 1, 2) - 1;
  if (u <= middle && middle <= j - 1) {
    return middle;
  }
  if (middle < u) {
    return u;
  }
  return j - 1;
}

int ApproximateMedian(int* array, int left, int right);

// returns an index to a kth smallest element (array will be modified!)
int KthSmallest(int* array, int left, int right, int k) {
  while (right - left >= kMinAmount) {
    // finds an approximate median index in [left, right] segment
    int median_index = ApproximateMedian(array, left, right);
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
  SortAndGetMedian(array, left, right);
  return left + k - 1;
}

// returns an approximate median of an array (array will be modified!)
int ApproximateMedian(int* array, int left, int right) {
  for (int i = left; i <= right; i += kMinAmount) {
    int sub_right = (i + kMinAmount < right) ? i + kMinAmount - 1 : right;
    int median_index = SortAndGetMedian(array, i, sub_right);
    std::swap(array[median_index], array[left + (i - left) / kMinAmount]);
  }
  int new_right = left + CeilDivide(right - left + 1, kMinAmount) - 1;
  int middle_position = CeilDivide(new_right - left + 1, 2);
  return KthSmallest(array, left, new_right, middle_position);
}

int main() {
  int n;
  int k;
  std::cin >> n >> k;
  int* array = new int[n];
  std::cin >> array[0] >> array[1];
  for (int i = 2; i < n; ++i) {
    array[i] = (array[i - 1] * 123 + array[i - 2] * 45) % kMod;
  }
  std::cout << array[KthSmallest(array, 0, n - 1, k)] << std::endl;
}
