#include <algorithm>
#include <iostream>

enum class Color : short { White, Grey, Black };

struct Vertex {
  int id, new_id;
  int l, r;  // segment of neighbours
  Color color = Color::White;
  int tout = -1;       // time out
  int component = -1;  // component with strong connection
};

void DFS(int i, std::pair<int, int>* array, Vertex* vertexes,
         int component = -1) {  // i in 1-indexation
  static int tout = 0;
  Vertex& v = vertexes[i - 1];
  v.color = Color::Grey;
  v.component = component;
  for (int j = v.l; j < v.r; ++j) {
    Vertex& v_neighbour = vertexes[array[j].second - 1];
    if (v_neighbour.color != Color::White) {
      continue;
    }
    DFS(array[j].second, array, vertexes, component);
  }
  v.color = Color::Black;
  v.tout = tout++;
}

void BuildEdges(int n, int m, Vertex* vertexes, std::pair<int, int>* array,
                bool change_id = false) {
  for (int i = 0, current = 0; i <= m; ++i) {
    while (current < n && (i == m || current != array[i].first)) {
      if (current > 0) {
        vertexes[current - 1].r = i;
      }
      ++current;
      vertexes[current - 1].l = i;
      if (change_id) {
        vertexes[current - 1].id = current;
      }
    }
  }
  vertexes[n - 1].r = m;
}

int main() {
  // speeding up
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  // initialization
  int n, m;
  std::cin >> n >> m;
  std::pair<int, int>* array = new std::pair<int, int>[m];
  Vertex* vertexes = new Vertex[n];
  for (int i = 0; i < m; ++i) {
    std::cin >> array[i].first >> array[i].second;
  }
  std::sort(array, array + m);
  BuildEdges(n, m, vertexes, array, true);
  // launch dfs on every vertex
  for (int i = 0; i < n; ++i) {
    if (vertexes[i].color == Color::White) {
      DFS(i + 1, array, vertexes);
    }
  }
  // inverse every edge
  for (int i = 0; i < m; ++i) {
    std::swap(array[i].first, array[i].second);
  }
  std::sort(array, array + m);
  BuildEdges(n, m, vertexes, array);
  // sort by time out (in decreasing order)
  std::sort(vertexes, vertexes + n,
            [](const Vertex& v1, const Vertex& v2) -> bool {
              return v1.tout >= v2.tout;
            });
  for (int i = 0; i < n; ++i) {
    vertexes[vertexes[i].id - 1].new_id = i + 1;
  }
  for (int i = 0; i < m; ++i) {
    array[i].second = vertexes[array[i].second - 1].new_id;
  }
  // paints everything in white
  for (int i = 0; i < n; ++i) {
    vertexes[i].color = Color::White;
  }
  // launch dfs on every vertex
  int amount_of_components = 0;
  for (int i = 0; i < n; ++i) {
    if (vertexes[i].color == Color::White) {
      DFS(i + 1, array, vertexes, ++amount_of_components);
    }
  }
  std::cout << amount_of_components << std::endl;
  std::sort(
      vertexes, vertexes + n,
      [](const Vertex& v1, const Vertex& v2) -> bool { return v1.id < v2.id; });
  for (int i = 0; i < n; ++i) {
    std::cout << vertexes[i].component << " ";
  }
  // freeing up memory
  delete[] array;
  delete[] vertexes;
}
