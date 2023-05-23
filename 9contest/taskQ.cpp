#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <climits>
#include <set>


class MinCostMaxFlowSolver {
    struct Edge {
        int v_from, v_to;
        int capacity = 0;
        int stream = 0;
        int cost = 0;
        bool is_real = true;
        //
        int reverse_edge_id = -1;

        int StreamRemains() const {
            return capacity - stream;
        }

        bool IsFull() const {
            return StreamRemains() == 0;
        }

        long long GetCost() const {
            return static_cast<long long>(stream) * cost;
        }
    };

    struct Vertex {
        std::vector<int> edges;
        int previous_edge = -1;
        int distance = INT_MAX;
        int distance_dump = INT_MAX;
        
        bool Reachable() const {
            return previous_edge != -1;
        }
    };

    int number_of_vertices_, number_of_edges_;
    std::vector<Edge> edges_;
    std::vector<Vertex> vertices_;

    void AddEdge(int v_from, int v_to, int capacity, int cost) {
        // adding real edge
        edges_.push_back(Edge{v_from, v_to, capacity, 0, cost, true, static_cast<int>(edges_.size()) + 1});
        vertices_[v_from].edges.push_back(static_cast<int>(edges_.size() - 1));
        // adding virtual edge
        edges_.push_back(Edge{v_to, v_from, 0, 0, -cost, false, static_cast<int>(edges_.size()) - 1});
        vertices_[v_to].edges.push_back(static_cast<int>(edges_.size() - 1));
    }

    void FordBellman(int v_from) {
        // initialization
        vertices_[v_from].distance_dump = 0;
        vertices_[v_from].distance = 0;
        for (int i = 1; i < number_of_vertices_; ++i) {
            // copy
            for (int j = 0; j < number_of_vertices_; ++j) {
                vertices_[j].distance_dump = vertices_[j].distance;
            }
            // expand vertices
            for (int j = 0; j < number_of_vertices_; ++j) {
                if (vertices_[j].distance_dump == INT_MAX) {
                    continue;
                }
                for (int edge_id : vertices_[j].edges) {
                    const Edge& edge = edges_[edge_id];
                    if (edge.IsFull()) { continue; }
                    int new_distance = vertices_[j].distance_dump + edge.cost;
                    if (new_distance < vertices_[edge.v_to].distance) {
                        vertices_[edge.v_to].distance = new_distance;
                        vertices_[edge.v_to].previous_edge = edge_id;
                    }
                }
            }
        }
    }

    void Clear() {
        for (int i = 0; i < number_of_vertices_; ++i) {
            vertices_[i].previous_edge = -1;
            vertices_[i].distance = INT_MAX;
            vertices_[i].distance_dump = INT_MAX;
        }
    }

  public:
    MinCostMaxFlowSolver(int number_of_vertices, int number_of_edges) : 
        number_of_vertices_(number_of_vertices),
        number_of_edges_(number_of_edges) {
            vertices_.assign(number_of_vertices_, {});
        }

    void Read(std::istream& stream) {
        for (int i = 0; i < number_of_edges_; ++i) {
            int v_from, v_to, capacity, cost;
            stream >> v_from >> v_to >> capacity >> cost;
            --v_from; --v_to;
            AddEdge(v_from, v_to, capacity, cost);
        }
    }

    long long FindMaxStream(int v_from, int v_to) {
        --v_from; --v_to;
        long long result = 0;
        // finds paths
        while (true) {
            FordBellman(v_from);
            if (!vertices_[v_to].Reachable()) {
                break;
            }
            int min_edge = INT_MAX;
            int vertex = v_to;
            while (vertex != v_from) {
                Edge& edge = edges_[vertices_[vertex].previous_edge];
                min_edge = std::min(min_edge, edge.StreamRemains());
                vertex = edge.v_from;
            }
            // correcting a found path
            vertex = v_to;
            while (vertex != v_from) {
                Edge& edge = edges_[vertices_[vertex].previous_edge];
                if (edge.is_real) {
                    edge.stream += min_edge;
                    edges_[edge.reverse_edge_id].capacity += min_edge;
                } else {
                    edge.capacity -= min_edge;
                    edges_[edge.reverse_edge_id].stream -= min_edge;
                }
                result += static_cast<long long>(min_edge) * edge.cost;
                vertex = edge.v_from;
            }
            // clearing
            Clear();
        }
        // finding the result
        return result;
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
    MinCostMaxFlowSolver solver(number_of_vertices, number_of_edges);
    solver.Read(std::cin);
    // finding the result
    std::cout << solver.FindMaxStream(1, number_of_vertices);
}
