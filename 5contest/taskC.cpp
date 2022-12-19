#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

class SplayTree {
  using ValueType = std::pair<std::string*, std::string*>;
  struct Node {
    ValueType value;
    Node* left = nullptr;
    Node* right = nullptr;

    ~Node() {
      delete left;
      delete right;
    }

    Node* FindPlace(const std::string& str) {
      if (left != nullptr && str < *value.first) {
        return left->FindPlace(str);
      }
      if (right != nullptr && *value.first < str) {
        return right->FindPlace(str);
      }
      return this;
    }
  };

  Node* root_ = nullptr;

  static Node* Zig(Node* node) {
    Node* new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;
    return new_root;
  }

  static Node* Zag(Node* node) {
    Node* new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;
    return new_root;
  }

  static Node* ZagZig(Node* node) {
    node->left = Zag(node->left);
    return Zig(node);
  }

  static Node* ZigZag(Node* node) {
    node->right = Zig(node->right);
    return Zag(node);
  }

  Node* Splay(Node* node, Node* x) {  // returns new root
    if (*x->value.first < *node->value.first) {
      node->left = Splay(node->left, x);
      // one rotation case
      if (node->left == x) {
        if (node == root_) {
          return root_ = Zig(node);  // x is returned
        }
        return node;
      }
      // two rotation cases
      if (*x->value.first < *node->left->value.first) {
        Zig(Zig(node));  // x is returned
      } else {
        ZagZig(node);  // x is returned
      }
      if (node == root_) {
        root_ = x;
      }
      return x;
    }
    if (*node->value.first < *x->value.first) {
      node->right = Splay(node->right, x);
      // one rotation case
      if (node->right == x) {
        if (node == root_) {
          return root_ = Zag(node);  // x is returned
        }
        return node;
      }
      // two rotation cases
      if (*node->right->value.first < *x->value.first) {
        Zag(Zag(node));  // x is returned
      } else {
        ZigZag(node);  // x is returned
      }
      if (node == root_) {
        root_ = x;
      }
      return x;
    }
    return node;
  }

 public:
  ~SplayTree() { delete root_; }

  void Add(ValueType value) {
    if (root_ == nullptr) {
      root_ = new Node{value};
      return;
    }
    Node* node = root_->FindPlace(*value.first);
    if (*node->value.first == *value.first) {
      Splay(root_, node);
      return;
    }
    if (*value.first < *node->value.first) {
      Splay(root_, node->left = new Node{value});
      return;
    }
    Splay(root_, node->right = new Node{value});
  }

  std::string* Find(const std::string& str) {
    if (root_ != nullptr) {
      Node* node = root_->FindPlace(str);
      Splay(root_, node);
      if (*node->value.first == str) {
        return node->value.second;
      }
    }
    return nullptr;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, q;
  std::string str, str1, str2;
  std::cin >> n;
  std::string* array = new std::string[2 * n];
  SplayTree tree1, tree2;
  for (int i = 0; i < n; ++i) {
    std::cin >> str1 >> str2;
    array[2 * i] = str1;
    array[2 * i + 1] = str2;
    tree1.Add({&array[2 * i], &array[2 * i + 1]});
    tree2.Add({&array[2 * i + 1], &array[2 * i]});
  }
  std::cin >> q;
  while (--q != -1) {
    std::cin >> str;
    std::string* result = tree1.Find(str);
    if (result == nullptr) {
      result = tree2.Find(str);
    }
    std::cout << *result << std::endl;
  }
  delete[] array;
}
