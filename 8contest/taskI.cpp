#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <type_traits>

class TagSolver;

class Permutation {
  struct Vector2d {
    int x;
    int y;
  };

  static const int kSide = 4;
  static std::map<char, Vector2d> k_directions;
  static std::map<char, char> k_directions_opposite;

  struct PermutationId {
    static const int kSide = Permutation::kSide;
    static const long long kGap = 100LL;
    static constexpr const long long kId[2] = {102030405060708LL,
                                               910111213141500LL};

    long long id[2] = {kId[0], kId[1]};

    static long long GetModule(int row, int col) {
      return static_cast<long long>(
          std::pow(kGap, ((kSide / 2) - (row % 2)) * kSide - col - 1));
    }

    int Get(int row, int col) const {
      long long mod = GetModule(row, col);
      return (id[row / 2] % (mod * kGap)) / mod;
    }

    void Set(int row, int col, int value) {
      id[row / 2] += (value - Get(row, col)) * GetModule(row, col);
    }
  };

  Vector2d zero_position_ = {kSide - 1, kSide - 1};

  PermutationId id_;
  int heuristic_ = 0;

 public:
  Permutation() {}

  void Read(std::istream& stream) {
    int number;
    for (int i = 0; i < kSide; ++i) {
      for (int j = 0; j < kSide; ++j) {
        stream >> number;
        if (number == 0) {
          zero_position_ = {i, j};
        }
        id_.Set(i, j, number);
      }
    }
    heuristic_ = CalculateHeuristic();
  }

  int Get(int row, int col) const { return id_.Get(row, col); }

  void Set(int row, int col, int value) { id_.Set(row, col, value); }

  Permutation& Move(char direction) {
    Vector2d result_pos = zero_position_ + k_directions[direction];
    if ((result_pos.x < 0 || kSide <= result_pos.x) ||
        (result_pos.y < 0 || kSide <= result_pos.y)) {
      return *this;
    }
    Set(zero_position_.x, zero_position_.y, Get(result_pos.x, result_pos.y));
    Set(result_pos.x, result_pos.y, 0);
    zero_position_ = result_pos;
    heuristic_ = CalculateHeuristic();
    return *this;
  }

  Permutation GetNeighbour(char direction) const {
    Permutation neighbour = *this;
    neighbour.Move(direction);
    return neighbour;
  }

  int GetSnakely(int index) const {
    int row = index / kSide;
    return Get(
        row, (row % 2 == 0) ? (index % kSide) : (kSide - (index % kSide) - 1));
  }

  PermutationId GetId() const { return id_; }

  int GetHeuristic() const { return heuristic_; }

  bool CalculateParity() const {
    bool parity = true;
    for (int i = 0; i < kSide * kSide; ++i) {
      for (int j = 0; j < i; ++j) {
        if (GetSnakely(i) == 0 || GetSnakely(j) == 0) {
          continue;
        }
        parity ^= static_cast<int>(GetSnakely(j) > GetSnakely(i));
      }
    }
    return parity;
  }

  static Vector2d GetActualPosition(int number) {
    if (number == 0) {
      return {kSide - 1, kSide - 1};
    }
    return {(number - 1) / kSide, (number - 1) % kSide};
  }

  int CalculateHeuristic() const {
    // Linear Conflict heuristic implementation
    int result = 0;
    for (int index = 0; index < kSide * kSide; ++index) {
      int row = index / kSide;
      int col = index % kSide;
      int number = Get(row, col);
      if (number == 0) {
        continue;
      }
      Vector2d pos = GetActualPosition(number);
      // Manhattan distance
      result += std::abs(row - pos.x) + std::abs(col - pos.y);
      // Conflicts detection
      if (0 < row && pos.y == col) {
        int number_up = Get(row - 1, col);
        if (number_up != 0) {
          Vector2d pos_up = GetActualPosition(number_up);
          if (pos_up.y == col && pos.x < pos_up.x) {
            result += 2;
          }
        }
      }
      if (0 < col && pos.x == row) {
        int number_left = Get(row, col - 1);
        if (number_left != 0) {
          Vector2d pos_left = GetActualPosition(number_left);
          if (pos_left.x == row && pos.y < pos_left.y) {
            result += 2;
          }
        }
      }
    }
    return result;
  }

  friend TagSolver;
  friend Permutation::Vector2d operator+(const Permutation::Vector2d& vector1,
                                         const Permutation::Vector2d& vector2);
  friend bool operator==(const Permutation::PermutationId& obj1,
                         const Permutation::PermutationId& obj2);
  friend bool operator<(const Permutation::PermutationId& obj1,
                        const Permutation::PermutationId& obj2);
};

bool operator==(const Permutation::PermutationId& obj1,
                const Permutation::PermutationId& obj2) {
  return (obj1.id[0] == obj2.id[0]) && (obj1.id[1] == obj2.id[1]);
}

bool operator<(const Permutation::PermutationId& obj1,
               const Permutation::PermutationId& obj2) {
  return (obj1.id[0] < obj2.id[0]) ||
         (obj1.id[0] == obj2.id[0] && obj1.id[1] < obj2.id[1]);
}

bool operator==(const Permutation& obj1, const Permutation& obj2) {
  return obj1.GetId() == obj2.GetId();
}

bool operator!=(const Permutation& obj1, const Permutation& obj2) {
  return !(obj1 == obj2);
}

Permutation::Vector2d operator+(const Permutation::Vector2d& vector1,
                                const Permutation::Vector2d& vector2) {
  return Permutation::Vector2d{vector1.x + vector2.x, vector1.y + vector2.y};
}

std::map<char, Permutation::Vector2d> Permutation::k_directions = {
    {'U', Vector2d{-1, 0}},
    {'L', Vector2d{0, -1}},
    {'D', Vector2d{1, 0}},
    {'R', Vector2d{0, 1}}};

std::map<char, char> Permutation::k_directions_opposite = {
    {'U', 'D'}, {'L', 'R'}, {'D', 'U'}, {'R', 'L'}};

class TagSolver {  // A* algorithm inside
  using IdType =
      std::result_of<decltype (&Permutation::GetId)(Permutation)>::type;

  struct QueueItem {
    long long distance;
    Permutation permutation;

    bool operator<(const QueueItem& item) const {
      return distance + static_cast<long long>(permutation.heuristic_) <
                 item.distance +
                     static_cast<long long>(item.permutation.heuristic_) ||
             (distance + static_cast<long long>(permutation.heuristic_) ==
                  item.distance +
                      static_cast<long long>(item.permutation.heuristic_) &&
              permutation.GetId() < item.permutation.GetId());
    };
  };

  struct MapItem {
    long long distance;
    char previous = '\0';
  };

  Permutation initial_permutation_;
  const Permutation kResultPermutation = Permutation();

  std::map<IdType, MapItem> distances_;

 public:
  TagSolver() {}

  void Read(std::istream& stream) { initial_permutation_.Read(stream); }

  int Find() {
    if (initial_permutation_.CalculateParity()) {
      return -1;
    }
    distances_[initial_permutation_.GetId()] = {0LL, '\n'};
    std::set<QueueItem> ordered_queue;
    ordered_queue.insert(QueueItem{0LL, initial_permutation_});
    while (!ordered_queue.empty()) {
      QueueItem item = *ordered_queue.begin();
      ordered_queue.erase(ordered_queue.begin());
      distances_[item.permutation.GetId()].distance = item.distance;
      if (item.permutation == kResultPermutation) {
        break;
      }
      // process neighbours
      for (char direction : "LURD") {
        if (direction == '\0') {
          continue;
        }
        Permutation neighbour = item.permutation.GetNeighbour(direction);
        if (item.permutation == neighbour) {
          continue;
        }
        if (distances_.find(neighbour.GetId()) == distances_.end()) {
          distances_[neighbour.GetId()] = {LLONG_MAX, '\0'};
        }
        if (item.distance + 1LL < distances_[neighbour.GetId()].distance) {
          auto it = ordered_queue.find(
              QueueItem{distances_[neighbour.GetId()].distance, neighbour});
          if (it != ordered_queue.end()) {
            ordered_queue.erase(it);
          }
          distances_[neighbour.GetId()] = {
              item.distance + 1LL,
              Permutation::k_directions_opposite[direction]};
          ordered_queue.insert(
              QueueItem{distances_[neighbour.GetId()].distance, neighbour});
        }
      }
    }
    return distances_[kResultPermutation.GetId()].distance;
  }

  std::string GetPath() {
    std::string path;
    Permutation permutation = kResultPermutation;
    while (permutation != initial_permutation_) {
      char direction = distances_[permutation.GetId()].previous;
      permutation.Move(direction);
      path.push_back(direction);
    }
    std::reverse(path.begin(), path.end());
    return path;
  }
};

int main() {
  TagSolver solver;
  solver.Read(std::cin);
  int result = solver.Find();
  std::cout << result << std::endl;
  if (result != -1) {
    std::cout << solver.GetPath();
  }
  // Attempt 3
}
