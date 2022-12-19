#include <algorithm>
#include <cmath>
#include <iostream>

class AvlTree {
  struct Node {
    int value;
    Node* left = nullptr;
    Node* right = nullptr;
    int rank = 0;
    int min = value, max = value;

    ~Node() {
      delete left;
      delete right;
    }

    inline int GetRankLeft() const {
      return (left != nullptr) ? left->rank : -1;
    }

    inline int GetRankRight() const {
      return (right != nullptr) ? right->rank : -1;
    }

    inline int GetDifference() const { return GetRankLeft() - GetRankRight(); }

    void CheckRank() { rank = std::max(GetRankLeft(), GetRankRight()) + 1; }

    void CheckMinMax() {
      min = (left != nullptr) ? left->min : value;
      max = (right != nullptr) ? right->max : value;
    }

    void Check() {
      CheckRank();
      CheckMinMax();
    }

    Node* FindNearest(int value) {
      if (this->value == value) {
        return this;
      }
      if (value < this->value) {
        if (left != nullptr && value <= left->max) {
          return left->FindNearest(value);
        }
        return this;
      }
      if (right != nullptr && value <= right->max) {
        return right->FindNearest(value);
      }
      return nullptr;
    }

    Node* FindPlace(int value) {
      if (this->value == value) {
        return this;
      }
      if (value < this->value) {
        if (left != nullptr) {
          return left->FindPlace(value);
        }
        return this;
      }
      if (right != nullptr) {
        return right->FindPlace(value);
      }
      return this;
    }
  };

  Node* root_ = nullptr;

  static Node* LeftRotation(Node* node) {
    Node* new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;
    node->Check();
    new_root->Check();
    return new_root;
  }

  static Node* RightRotation(Node* node) {
    Node* new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;
    node->Check();
    new_root->Check();
    return new_root;
  }

  Node* Balance(Node* node, int value) {
    if (value == node->value) {
      return node;
    }
    if (value < node->value) {
      node->left = Balance(node->left, value);
    }
    if (node->value < value) {
      node->right = Balance(node->right, value);
    }
    node->Check();
    int diff = node->GetDifference();
    if (diff == 2) {
      if (node->left->GetDifference() == -1) {
        node->left = RightRotation(node->left);
      }
      return LeftRotation(node);
    }
    if (diff == -2) {
      if (node->right->GetDifference() == 1) {
        node->right = LeftRotation(node->right);
      }
      return RightRotation(node);
    }
    return node;
  }

 public:
  ~AvlTree() { delete root_; }

  void Add(int value) {
    if (root_ == nullptr) {
      root_ = new Node{value};
      return;
    }
    Node* node = root_->FindPlace(value);
    if (value == node->value) {
      return;
    }
    if (value < node->value) {
      node->left = new Node{value};
    } else {
      node->right = new Node{value};
    }
    node->Check();
    root_ = Balance(root_, node->value);
  }

  int Find(int value) const {
    if (root_ == nullptr) {
      return -1;
    }
    Node* node = root_->FindNearest(value);
    return (node != nullptr) ? node->value : -1;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n;
  int i;
  int y;
  char previous_cmd = 0;
  char cmd = 0;
  const int kAntiOfflineModule = static_cast<int>(std::pow(10, 9));
  AvlTree tree;
  std::cin >> n;
  while (--n != -1) {
    std::cin >> cmd >> i;
    if (cmd == '?') {
      std::cout << (y = tree.Find(i)) << std::endl;
    } else {
      if (previous_cmd == '?') {
        i = (i + y) % kAntiOfflineModule;
      }
      tree.Add(i);
    }
    previous_cmd = cmd;
  }
}
