#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

template <typename T, typename H, typename Func>
int BinarySearch(const T* array, int size, const H& value, Func get);

struct Point {
  int x, y, w, ix, iy, i;  // (x, y, w, x_compressed, y_compressed, i)
};

bool operator<(const Point& p1, const Point& p2);

class Fenwick {
  bool initialized_ = false;
  int n_;
  std::pair<int, long long>* array_;

  void UpdateByI(int i, int delta);

 public:
  Fenwick(const Point* begin, int n);

  ~Fenwick();

  void Update(int iy, int delta);

  long long Sum(int iy) const;
};

class FenwickTreeOfFenwickTrees {
  std::vector<Fenwick> array_;

 public:
  FenwickTreeOfFenwickTrees(const std::vector<Point>& array);

  void Update(size_t ix, int iy, int delta);

  long long Sum(int ix, int iy) const;
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, x, y, w;
  std::cin >> n;
  std::vector<Point> points;
  for (int i = 0; i < n; ++i) {
    std::cin >> x >> y >> w;
    points.push_back(Point{x, y, w, -1, -1, i});
  }
  std::sort(
      points.begin(), points.end(),
      [](const Point& p1, const Point& p2) -> bool { return p1.y < p2.y; });
  std::vector<int> array_y_sorted;
  for (int i = 0; i < n; ++i) {
    points[i].iy = i;
    array_y_sorted.push_back(points[i].y);
  }
  std::sort(points.begin(), points.end());
  int* indices = new int[n];
  for (int i = 0; i < n; ++i) {
    points[i].ix = i;
    indices[points[i].i] = i;
  }
  FenwickTreeOfFenwickTrees fenwick(points);
  //
  int m, rx, ry, i, z;
  std::string cmd;
  std::cin >> m;
  while (--m != -1) {
    std::cin >> cmd;
    if (cmd == "get") {
      std::cin >> rx >> ry;
      int ix = BinarySearch(points.data(), n, rx,
                            [](const Point& p) -> int { return p.x; });
      int iy = BinarySearch(array_y_sorted.data(), n, ry,
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
  delete[] indices;
}

template <typename T, typename H, typename Func>
int BinarySearch(const T* array, int size, const H& value, Func get) {
  if (get(array[0]) > value) {
    return -1;
  }
  int left = 0, right = size - 1;
  while (right - left > 1) {
    int middle = (left + right) / 2;
    if (get(array[middle]) <= value) {
      left = middle;
    } else {
      right = middle - 1;
    }
  }
  if (get(array[right]) <= value) {
    return right;
  }
  return left;
}

bool operator<(const Point& p1, const Point& p2) { return p1.x < p2.x; }

void Fenwick::UpdateByI(int i, int delta) {
  for (; i < n_; i = i | (i + 1)) {
    array_[i].second += delta;
  }
}

Fenwick::Fenwick(const Point* begin, int n)
    : n_(n), array_(new std::pair<int, long long>[n_]) {
  initialized_ = true;
  for (int i = 0; i < n_; ++i) {
    array_[i] = {begin[i].iy, 0};
  }
  std::sort(array_, array_ + n_);
  for (int i = 0; i < n_; ++i) {
    Update(begin[i].iy, begin[i].w);
  }
}

Fenwick::~Fenwick() { delete[] array_; }

void Fenwick::Update(int iy, int delta) {
  int i = BinarySearch(
      array_, n_, iy,
      [](const std::pair<int, long long>& p) -> int { return p.first; });
  UpdateByI(i, delta);
}

long long Fenwick::Sum(int iy) const {
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

FenwickTreeOfFenwickTrees::FenwickTreeOfFenwickTrees(
    const std::vector<Point>& array) {
  array_.reserve(array.size());
  for (size_t i = 0; i < array.size(); ++i) {
    array_.emplace_back(array.data() + (i & (i + 1)), i - (i & (i + 1)) + 1);
  }
}

void FenwickTreeOfFenwickTrees::Update(size_t ix, int iy, int delta) {
  for (; ix < array_.size(); ix = ix | (ix + 1)) {
    array_[ix].Update(iy, delta);
  }
}

long long FenwickTreeOfFenwickTrees::Sum(int ix, int iy) const {
  long long result = 0;
  for (; ix >= 0; ix = (ix & (ix + 1)) - 1) {
    result += array_[ix].Sum(iy);
  }
  return result;
}
