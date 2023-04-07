#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

class Graph {
 public:
  struct Edge {
    int id;
    int v_to;
    int weight;

    bool operator<(const Edge& edge) const { return id < edge.id; };
  };

  struct Vertex {
    int id;
    std::set<Edge> neighbours;
  };

 private:
  int amount_vertices_;  // amount of vertexes, edges
  Vertex* vertices_;

 public:
  Graph(int amount_vertices)
      : amount_vertices_(amount_vertices),
        vertices_(new Vertex[amount_vertices]) {
    for (int index = 0; index < amount_vertices_; ++index) {
      vertices_[index].id = index;
    }
  }

  ~Graph() { delete[] vertices_; }

  void ReadAdjacencyMatrix(std::istream& stream, int unattainable_value) {
    int weight;
    // assumes 0-indexation
    for (int i = 0; i < amount_vertices_; ++i) {
      for (int j = 0; j < amount_vertices_; ++j) {
        stream >> weight;
        if (weight == unattainable_value) {
          continue;
        }
        vertices_[i].neighbours.insert(
            Edge{i * amount_vertices_ + j, j, weight});
      }
    }
  }

  Vertex& operator[](int index) { return vertices_[index]; }
};

class ShortestPathFinder {
  struct DPItem {
    int distance;
    int vertex_id;
    DPItem* previous = nullptr;  // previous vertex (id)
  };

  int amount_vertices_;
  Graph graph_;

  DPItem** dp_ = nullptr;

  DPItem* start_vertex_ = nullptr;

 public:
  ShortestPathFinder(int amount_vertices)
      : amount_vertices_(amount_vertices),
        graph_(amount_vertices),
        dp_(new DPItem*[amount_vertices + 1]) {
    for (int i = 0; i <= amount_vertices_; ++i) {
      dp_[i] = new DPItem[amount_vertices_];
    }
  }

  ~ShortestPathFinder() {
    for (int i = 0; i <= amount_vertices_; ++i) {
      delete[] dp_[i];
    }
    delete[] dp_;
  }

  void ReadAdjacencyMatrix(std::istream& stream, int unattainable_value) {
    graph_.ReadAdjacencyMatrix(stream, unattainable_value);
  }

  bool FindCycle() {
    for (int i = 0; i < amount_vertices_; ++i) {
      dp_[0][i] = {((i == 0) ? 0 : INT_MAX), i, nullptr};
    }
    // "i" represents acceptable amount of edges
    for (int i = 1; i <= amount_vertices_; ++i) {
      // copy
      for (int vertex_id = 0; vertex_id < amount_vertices_; ++vertex_id) {
        dp_[i][vertex_id] = dp_[i - 1][vertex_id];
      }
      for (int vertex_id = 0; vertex_id < amount_vertices_; ++vertex_id) {
        if (dp_[i - 1][vertex_id].distance == INT_MAX) {
          continue;
        }
        for (const Graph::Edge& edge : graph_[vertex_id].neighbours) {
          int new_distance = dp_[i - 1][vertex_id].distance + edge.weight;
          if (new_distance < dp_[i][edge.v_to].distance) {
            dp_[i][edge.v_to] = {new_distance, edge.v_to,
                                 &dp_[i - 1][vertex_id]};
          }
        }
      }
    }
    // compare
    for (int i = 0; i < amount_vertices_; ++i) {
      if (dp_[amount_vertices_][i].distance <
          dp_[amount_vertices_ - 1][i].distance) {
        start_vertex_ = &dp_[amount_vertices_][i];
        return true;
      }
    }
    return false;
  }

  void PrintCycle() {
    std::map<int, DPItem*> vertices_unique;
    vertices_unique[start_vertex_->vertex_id] = start_vertex_;
    DPItem* previous = start_vertex_->previous;
    while (vertices_unique.find(previous->vertex_id) == vertices_unique.end()) {
      vertices_unique[previous->vertex_id] = previous;
      previous = previous->previous;
    }
    start_vertex_ = vertices_unique.find(previous->vertex_id)->second;
    //
    std::vector<int> vertices;
    vertices.push_back(start_vertex_->vertex_id);
    previous = start_vertex_->previous;
    while (previous->vertex_id != start_vertex_->vertex_id) {
      vertices.push_back(previous->vertex_id);
      previous = previous->previous;
    }
    vertices.push_back(previous->vertex_id);
    std::cout << vertices.size() << std::endl;
    for (auto it = vertices.rbegin(); it != vertices.rend(); ++it) {
      std::cout << *it << " ";
    }
  }
};

int main() {
  // speeding up
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  // initialization
  static const int kUnattainable = 100'000;
  int amount_vertices;
  std::cin >> amount_vertices;
  ShortestPathFinder finder(amount_vertices + 1);
  // preprocessing (appending a zero vertex)
  std::stringstream stream;
  for (int i = 0; i < amount_vertices + 1; ++i) {
    stream << "0 ";
  }
  for (int i = 0; i < amount_vertices; ++i) {
    stream << kUnattainable << ' ';
    for (int j = 0; j < amount_vertices; ++j) {
      int weight;
      std::cin >> weight;
      stream << weight << ' ';
    }
  }
  // finding the result
  finder.ReadAdjacencyMatrix(stream, kUnattainable);
  if (finder.FindCycle()) {
    std::cout << "YES" << std::endl;
    finder.PrintCycle();
  } else {
    std::cout << "NO" << std::endl;
  }
}
