#include <iostream>
#include <algorithm>
#include <vector>


class Graph {
    enum class Colors { White, Grey, Black };

    struct Vertex {
        bool is_left = false;
        int in_match = -1; // -1 means that this vertex not in match
        Colors color = Colors::White;
        std::vector<int> neighbours;
    };

    int m_, n_;
    Vertex* vertices_;

    void DFS(int parent, int vertex) {
        vertices_[vertex].color = Colors::Grey;
        for (int neighbour : vertices_[vertex].neighbours) {
            if (neighbour == parent || vertices_[neighbour].color != Colors::White) {
                continue;
            }
            if (vertices_[vertex].in_match == neighbour && vertices_[vertex].is_left) {
                continue;
            }
            if (vertices_[vertex].in_match != neighbour && !vertices_[vertex].is_left) {
                continue;
            }
            DFS(vertex, neighbour);
        }
        vertices_[vertex].color = Colors::Black;
    }

  public:
    Graph(int m, int n) : m_(m), n_(n), vertices_(new Vertex[m + n]) {}

    void Read(std::istream& stream) {
        for (int i = 0; i < m_; ++i) {
            vertices_[i].is_left = true;
            int amount;
            stream >> amount;
            for (int j = 0; j < amount; ++j) {
                int vertex;
                stream >> vertex;
                vertices_[i].neighbours.push_back(m_ + vertex - 1);
                vertices_[m_ + vertex - 1].neighbours.push_back(i);
            }
        }
        for (int i = 0; i < m_; ++i) {
            int vertex;
            stream >> vertex;
            if (vertex == 0) { continue; }
            vertices_[i].in_match = vertex - 1;
            vertices_[m_ + vertex - 1].in_match = i;
        }
    }

    void Find() {
        for (int i = 0; i < m_; ++i) {
            if (vertices_[i].in_match == -1) {
                DFS(-1, i);
            }
        }
    }

    void PrintVertexCover(std::ostream& stream) const {
        int l_minus = 0;
        int r_plus = 0;
        for (int i = 0; i < m_; ++i) {
            if (vertices_[i].color == Colors::White) {
                ++l_minus;
            }
        }
        for (int i = 0; i < n_; ++i) {
            if (vertices_[m_ + i].color == Colors::Black) {
                ++r_plus;
            }
        }
        stream << l_minus + r_plus << std::endl;
        stream << l_minus << " ";
        for (int i = 0; i < m_; ++i) {
            if (vertices_[i].color == Colors::White) {
                stream << i + 1 << " ";
            }
        }
        stream << std::endl;
        stream << r_plus << " ";
        for (int i = 0; i < n_; ++i) {
            if (vertices_[m_ + i].color == Colors::Black) {
                stream << i + 1 << " ";
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
    int m, n;
    std::cin >> m >> n;
    Graph graph(m, n);
    graph.Read(std::cin);
    // finding the result
    graph.Find();
    // printing the result
    graph.PrintVertexCover(std::cout);
}
