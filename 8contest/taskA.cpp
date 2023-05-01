#include <algorithm>
#include <climits>
#include <iostream>
#include <set>

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
  int amount_vertexes_, amount_edges_;  // amount of vertexes, edges
  Vertex* vertexes_;

 public:
  Graph(int amount_vertexes, int amount_edges)
      : amount_vertexes_(amount_vertexes),
        amount_edges_(amount_edges),
        vertexes_(new Vertex[amount_vertexes]) {
    for (int index = 0; index < amount_vertexes_; ++index) {
      vertexes_[index].id = index;
    }
  }

  ~Graph() { delete[] vertexes_; }

  void ReadEdges(std::istream& stream) {
    int v_from;
    int v_to;
    int weight;
    // assumes 0-indexation
    for (int index = 0; index < amount_edges_; ++index) {
      stream >> v_from >> v_to >> weight;
      vertexes_[v_from].neighbours.insert(Edge{2 * index, v_to, weight});
      vertexes_[v_to].neighbours.insert(Edge{2 * index + 1, v_from, weight});
    }
  }

  Vertex& operator[](int index) { return vertexes_[index]; }
};

class ShortestPathFinder {  // Dijkstra algorithm inside
  struct QueueItem {
    int distance;
    int vertex_id;

    bool operator<(const QueueItem& item) const {
      return distance < item.distance ||
             (distance == item.distance && vertex_id < item.vertex_id);
    };
  };

  int amount_vertexes_;
  Graph graph_;
  int* distances_;
  std::set<QueueItem> ordered_queue_;

 public:
  ShortestPathFinder(int amount_vertexes, int amount_edges)
      : amount_vertexes_(amount_vertexes),
        graph_{amount_vertexes, amount_edges},
        distances_(new int[amount_vertexes]) {}

  ~ShortestPathFinder() { delete[] distances_; }

  void ReadEdges(std::istream& stream) { graph_.ReadEdges(stream); }

  void Find(int vertex_id) {
    for (int index = 0; index < amount_vertexes_; ++index) {
      distances_[index] = INT_MAX;
    }
    ordered_queue_.insert(QueueItem{0, vertex_id});
    while (!ordered_queue_.empty()) {
      QueueItem item = *ordered_queue_.begin();
      ordered_queue_.erase(ordered_queue_.begin());
      distances_[item.vertex_id] = item.distance;
      // process neighbours
      for (const Graph::Edge& edge : graph_[item.vertex_id].neighbours) {
        if (item.distance + edge.weight < distances_[edge.v_to]) {
          auto it =
              ordered_queue_.find(QueueItem{distances_[edge.v_to], edge.v_to});
          if (it != ordered_queue_.end()) {
            ordered_queue_.erase(it);
          }
          distances_[edge.v_to] = item.distance + edge.weight;
          ordered_queue_.insert(QueueItem{distances_[edge.v_to], edge.v_to});
        }
      }
    }
  }

  int operator[](int index) const { return distances_[index]; }
};

int main() {
  // speeding up
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  // initialization
  static const int kMax = 2'009'000'999;
  int amount_maps;
  int amount_vertexes;
  int amount_edges;
  int vertex_id;
  std::cin >> amount_maps;
  for (int i = 0; i < amount_maps; ++i) {
    std::cin >> amount_vertexes >> amount_edges;
    ShortestPathFinder finder(amount_vertexes, amount_edges);
    finder.ReadEdges(std::cin);
    std::cin >> vertex_id;
    finder.Find(vertex_id);
    for (int i = 0; i < amount_vertexes; ++i) {
      std::cout << ((finder[i] != INT_MAX) ? finder[i] : kMax) << " ";
    }
    std::cout << std::endl;
  }
}
