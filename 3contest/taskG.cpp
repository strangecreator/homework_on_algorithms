#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <tuple>

template <typename T, typename H, typename Func>
int BinarySearch(T* array, int size, H&& value, Func get) {
  if (get(array[0]) > std::forward<H>(value)) {
    return -1;
  }
  int left = 0, right = size - 1;
  while (right - left > 1) {
    int middle = (left + right) >> 1;
    if (get(array[middle]) <= std::forward<H>(value)) {
      left = middle;
    } else {
      right = middle - 1;
    }
  }
  if (get(array[right]) <= std::forward<H>(value)) {
    return right;
  }
  return left;
}

struct Point {
  int x, y, w, ix, iy, i;  // (x, y, w, x_compressed, y_compressed, i)

  bool operator<(const Point& point) const { return x < point.x; }
};

class Fenwick {
  bool initialized_ = false;
  int n_;
  std::pair<int, long long>* array_;

  void UpdateByI(int i, int delta) {
    for (; i < n_; i = i | (i + 1)) {
      array_[i].second += delta;
    }
  }

 public:
  ~Fenwick() {
    if (initialized_) {
      delete[] array_;
    }
  }

  void Initialize(Point* begin, int n) {
    initialized_ = true;
    n_ = n;
    array_ = new std::pair<int, long long>[n_];
    for (int i = 0; i < n_; ++i) {
      array_[i] = {begin[i].iy, 0};
    }
    std::sort(array_, array_ + n_);
    for (int i = 0; i < n_; ++i) {
      Update(begin[i].iy, begin[i].w);
    }
  }

  void Update(int iy, int delta) {
    int i = BinarySearch(
        array_, n_, iy,
        [](const std::pair<int, long long>& p) -> int { return p.first; });
    UpdateByI(i, delta);
  }

  long long Sum(int iy) const {
    int i = BinarySearch(
        array_, n_, iy,
        [](const std::pair<int, long long>& p) -> int { return p.first; });
    long long result = 0;
    if (i != -1) {
      for (; i >= 0; i = (i & (i + 1)) - 1) {
        result += array_[i].second;
      }
    }
    return result;
  }
};

class FenwickTreeOfFenwickTrees {
  int n_;
  Fenwick* array_;

 public:
  FenwickTreeOfFenwickTrees(Point* array, int n)
      : n_(n), array_(new Fenwick[n_]) {
    for (int i = 0; i < n_; ++i) {
      array_[i].Initialize(array + (i & (i + 1)), i - (i & (i + 1)) + 1);
    }
  }

  ~FenwickTreeOfFenwickTrees() { delete[] array_; }

  void Update(int ix, int iy, int delta) {
    for (; ix < n_; ix = ix | (ix + 1)) {
      array_[ix].Update(iy, delta);
    }
  }

  long long Sum(int ix, int iy) const {
    long long result = 0;
    for (; ix >= 0; ix = (ix & (ix + 1)) - 1) {
      result += array_[ix].Sum(iy);
    }
    return result;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, x, y, w;
  std::cin >> n;
  Point* points = new Point[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> x >> y >> w;
    points[i] = {x, y, w, -1, -1, i};
  }
  std::sort(points, points + n, [](const Point& p1, const Point& p2) -> bool {
    return p1.y < p2.y;
  });
  int* array_y_sorted = new int[n];
  for (int i = 0; i < n; ++i) {
    points[i].iy = i;
    array_y_sorted[i] = points[i].y;
  }
  std::sort(points, points + n);
  int* indices = new int[n];
  for (int i = 0; i < n; ++i) {
    points[i].ix = i;
    indices[points[i].i] = i;
  }
  FenwickTreeOfFenwickTrees fenwick(points, n);
  //
  int m, rx, ry, i, z;
  std::string cmd;
  std::cin >> m;
  while (--m != -1) {
    std::cin >> cmd;
    if (cmd == "get") {
      std::cin >> rx >> ry;
      int ix = BinarySearch(points, n, rx,
                            [](const Point& p) -> int { return p.x; });
      int iy = BinarySearch(array_y_sorted, n, ry,
                            [](const int& y) -> int { return y; });
      if (ix == -1 || iy == -1) {
        std::cout << 0 << std::endl;
      } else {
        std::cout << fenwick.Sum(ix, iy) << std::endl;
      }
    } else {
      std::cin >> i >> z;
      int ix = indices[i - 1];
      int iy = points[ix].iy;
      fenwick.Update(ix, iy, z - points[ix].w);
      points[ix].w = z;
    }
  }
  delete[] points;
  delete[] array_y_sorted;
  delete[] indices;
}
