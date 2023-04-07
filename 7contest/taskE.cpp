#include <algorithm>
#include <iostream>

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

void DFS(Vertex* v, Vertex* parent, Edge* array) {
  static int tin = 0;
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
    DFS(v_neighbour, v, array);
    if (v_neighbour->ret == v_neighbour->tin) {
      array[j].is_bridge = true;
    }
    v->ret = std::min(v->ret, v_neighbour->ret);
  }
}

int main() {
  // speeding up
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  // initialization
  int n, m, v1, v2;
  std::cin >> n >> m;
  Vertex* vertexes = new Vertex[n];
  for (int i = 0; i < n; ++i) {
    vertexes[i].id = i + 1;
  }
  Edge* array = new Edge[2 * m];
  for (int i = 0; i < m; ++i) {
    std::cin >> v1 >> v2;
    array[2 * i] = {i + 1, &vertexes[v1 - 1], &vertexes[v2 - 1]};
    array[2 * i + 1] = {i + 1, &vertexes[v2 - 1], &vertexes[v1 - 1]};
  }
  // build edges
  std::sort(array, array + 2 * m, [](const Edge& e1, const Edge& e2) -> bool {
    return (e1.v1->id < e2.v1->id) ||
           (e1.v1->id == e2.v1->id && e1.v2->id <= e2.v2->id);
  });
  for (int i = 0, current = 0; i <= 2 * m; ++i) {
    while (current < n && (i == 2 * m || current != array[i].v1->id)) {
      if (current > 0) {
        vertexes[current - 1].r = i;
      }
      ++current;
      vertexes[current - 1].l = i;
    }
  }
  vertexes[n - 1].r = 2 * m;
  // finding the result
  for (int i = 0; i < n; ++i) {
    if (!vertexes[i].used) {
      DFS(&vertexes[i], nullptr, array);
    }
  }
  // printing the result
  std::sort(array, array + 2 * m, [](const Edge& e1, const Edge& e2) -> bool {
    return e1.id < e2.id;
  });
  int amount = 0;
  for (int i = 0; i < 2 * m; ++i) {
    if (array[i].is_bridge) {
      ++amount;
    }
  }
  std::cout << amount << std::endl;
  for (int i = 0; i < 2 * m; ++i) {
    if (array[i].is_bridge) {
      std::cout << array[i].id << " ";
    }
  }
  // freeing up memory
  delete[] array;
  delete[] vertexes;
}
