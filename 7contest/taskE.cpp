#include <algorithm>
#include <iostream>


class Graph {
  struct Vertex {
    int id;    // 1-indexation
    int l, r;  // segment of neighbours
    bool used = false;
    int tin;  // time in
    int ret;  // maximum vertex's time in
  };

  struct Edge {
    int id;  // 1-indexation
    Vertex* v1;
    Vertex* v2;
    bool is_bridge = false;
  };

  int n_, m_;
  Vertex* vertices_;
  Edge* edges_;

  void DFS(Vertex* v, Vertex* parent, Edge* array, int tin=0) {
    // static int tin = 0;
    v->used = true;
    v->tin = ++tin;
    v->ret = v->tin;
    bool parent_used = false;
    for (int j = v->l; j < v->r; ++j) {
      Vertex* v_neighbour = array[j].v2;
      if (parent == v_neighbour) {
        if (parent_used) {
          v->ret = std::min(v->ret, parent->tin);
        } else {
          parent_used = true;
        }
        continue;
      }
      if (v_neighbour->used) {
        v->ret = std::min(v->ret, v_neighbour->tin);
        continue;
      }
      DFS(v_neighbour, v, array, tin);
      if (v_neighbour->ret == v_neighbour->tin) {
        array[j].is_bridge = true;
      }
      v->ret = std::min(v->ret, v_neighbour->ret);
    }
  }

  void BuildEdges() {
    std::sort(edges_, edges_ + 2 * m_, [](const Edge& e1, const Edge& e2) -> bool {
      return (e1.v1->id < e2.v1->id) ||
            (e1.v1->id == e2.v1->id && e1.v2->id <= e2.v2->id);
    });
    for (int i = 0, current = 0; i <= 2 * m_; ++i) {
      while (current < n_ && (i == 2 * m_ || current != edges_[i].v1->id)) {
        if (current > 0) {
          vertices_[current - 1].r = i;
        }
        ++current;
        vertices_[current - 1].l = i;
      }
    }
    vertices_[n_ - 1].r = 2 * m_;
  }

 public:
  Graph(int n, int m) : n_(n), m_(m),
    vertices_(new Vertex[n]),
    edges_(new Edge[2 * m]) {
    for (int i = 0; i < n_; ++i) {
      vertices_[i].id = i + 1;
    }
  }

~Graph() {
    delete[] vertices_;
    delete[] edges_;
  }

  void Read(std::istream& stream) {
    for (int i = 0; i < m_; ++i) {
      int v1, v2;
      stream >> v1 >> v2;
      --v1; --v2;
      edges_[2 * i] = {i + 1, &vertices_[v1], &vertices_[v2]};
      edges_[2 * i + 1] = {i + 1, &vertices_[v2], &vertices_[v1]};
    }
    BuildEdges();
  }

  void FindBridges() {
    for (int i = 0; i < n_; ++i) {
      if (!vertices_[i].used) {
        DFS(&vertices_[i], nullptr, edges_);
      }
    }
  }

  void PrintBridges(std::ostream& stream) {
    std::sort(edges_, edges_ + 2 * m_, [](const Edge& e1, const Edge& e2) -> bool {
      return e1.id < e2.id;
    });
    int amount = 0;
    for (int i = 0; i < 2 * m_; ++i) {
      if (edges_[i].is_bridge) {
        ++amount;
      }
    }
    std::cout << amount << std::endl;
    for (int i = 0; i < 2 * m_; ++i) {
      if (edges_[i].is_bridge) {
        stream << edges_[i].id << " ";
      }
    }
  }
};

int main() {
  // speeding up
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  // initialization
  int n, m;
  std::cin >> n >> m;
  Graph graph(n, m);
  graph.Read(std::cin);
  // finding the result
  graph.FindBridges();
  // printing the result
  graph.PrintBridges(std::cout);
}
