#include <iostream>
#include <vector>
#include <algorithm>


class DSU { // disjoint set union
    struct Cell {
        int parent = -1;
        int rank = 1;
    };

    int number_of_vertices_;
    Cell* array_;

  public:
    DSU(int number_of_vertices) : 
        number_of_vertices_(number_of_vertices), 
        array_(new Cell[number_of_vertices]) {}

    ~DSU() {
        delete[] array_;
    }

    int GetRoot(int v_from) {
        if (array_[v_from].parent != -1) {
            array_[v_from].parent = GetRoot(array_[v_from].parent);
            return array_[v_from].parent;
        }
        return v_from;
    }

    void Union(int v_from, int v_to) {
        v_from = GetRoot(v_from);
        v_to = GetRoot(v_to);
        if (v_from == v_to) {
            return;
        }
        if (array_[v_from].rank <= array_[v_to].rank) {
            array_[v_from].parent = v_to;
            if (array_[v_from].rank == array_[v_to].rank) {
                ++array_[v_to].rank;
            }
        } else {
            array_[v_to].parent = v_from;
        }
    }

    bool IsConnected(int v_from, int v_to) {
        return GetRoot(v_from) == GetRoot(v_to);
    }
};

class Graph {
  public:
    struct Edge {
        int v_from;
        int v_to;
        int weight;
    };

  private:
    int number_of_vertices_;
    int number_of_edges_;
    Edge* edges_;

  public:
    Graph(int number_of_vertices, int number_of_edges) : 
        number_of_vertices_(number_of_vertices), 
        number_of_edges_(number_of_edges), 
        edges_(new Edge[number_of_edges]) {}

    ~Graph() {
        delete[] edges_;
    }

    void Read(std::istream& stream) {
        for (int i = 0; i < number_of_edges_; ++i) {
            int v_from;
            int v_to;
            int weight;
            stream >> v_from >> v_to >> weight;
            edges_[i] = {v_from - 1, v_to - 1, weight};
        }
    }

    void SortEdges() {
        std::sort(edges_, edges_ + number_of_edges_, [](const Edge& e1, const Edge& e2) -> bool {
            return e1.weight < e2.weight;
        });
    }

    Edge& GetEdge(int index) {
        return edges_[index];
    }
};

int main() {
    // speeding up
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    // initialization
    int number_of_vertices, number_of_edges;
    std::cin >> number_of_vertices >> number_of_edges;
    Graph graph(number_of_vertices, number_of_edges);
    graph.Read(std::cin);
    graph.SortEdges();
    DSU dsu(number_of_vertices);
    // finding the result
    long long result_cost = 0;
    for (int i = 0; i < number_of_edges; ++i) {
        int v_from = graph.GetEdge(i).v_from;
        int v_to = graph.GetEdge(i).v_to;
        if (!dsu.IsConnected(v_from, v_to)) {
            dsu.Union(v_from, v_to);
            result_cost += graph.GetEdge(i).weight;
        }
    }
    std::cout << result_cost;
}
