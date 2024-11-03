//
// Created by beto on 21/10/24.
//

#include "AVLTree.hpp"

#include <cmath>
#include <gsl/gsl>
#include <iostream>
#include <queue>
#include <stack>

int AVLTree::height(Node const * node) {
  return node == nullptr ? 0 : node->height;
}

int AVLTree::getHeight(Node const * node) {
  return height(node);
}

int AVLTree::max(int const param_a, int const param_b) {
  return param_a > param_b ? param_a : param_b;
}

int AVLTree::getMax(int const param_a, int const param_b) {
  return max(param_a, param_b);
}

Node * AVLTree::rotate_left(Node * param_x) {
  Node * param_y = param_x->right;
  Node * T_2     = param_y->left;

  param_y->left  = param_x;
  param_x->right = T_2;

  param_x->height = max(height(param_x->left), height(param_x->right)) + 1;
  param_y->height = max(height(param_y->left), height(param_y->right)) + 1;

  return param_y;
}

Node * AVLTree::get_rotate_left(Node * param_x) {
  return rotate_left(param_x);
}

Node * AVLTree::rotate_right(Node * value_y) {
  Node * value_x = value_y->left;
  Node * T_2     = value_x->right;

  value_x->right = value_y;
  value_y->left  = T_2;

  value_y->height = max(height(value_y->left), height(value_y->right)) + 1;
  value_x->height = max(height(value_x->left), height(value_x->right)) + 1;

  return value_x;
}

int AVLTree::get_balance(Node const * node) {
  return node == nullptr ? 0 : height(node->left) - height(node->right);
}

gsl::owner<Node *> AVLTree::insert(Node * node, element const elem) {
  if (node == nullptr) { return static_cast<gsl::owner<Node *>>(new Node(elem)); }

  Node * current = node;
  Node * parent  = nullptr;

  std::stack<Node *> path;

  while (current != nullptr) {
    parent = current;
    path.push(parent);

    if (elem.color < current->color) {
      current = current->left;
    } else if (elem.color > current->color) {
      current = current->right;
    } else {
      return nullptr;
    }
  }

  if (elem.color < parent->color) {
    parent->left = static_cast<gsl::owner<Node *>>(new Node(elem));
  } else {
    parent->right = static_cast<gsl::owner<Node *>>(new Node(elem));
  }

  while (!path.empty()) {
    Node * current = path.top();
    path.pop();

    current->height = 1 + max(height(current->left), height(current->right));

    int const balance = get_balance(current);

    if (balance > 1) {  // Caso izquierda
      if (elem.color < current->left->color) {
        // Caso 1: Desbalanceo izquierda-izquierda
        if (!path.empty()) {
          Node * parent = path.top();
          if (parent->left == current) {
            parent->left = rotate_right(current);
          } else {
            parent->right = rotate_right(current);
          }
        } else {
          node = rotate_right(current);
        }
      } else {
        // Caso 3: Desbalanceo izquierda-derecha
        current->left = rotate_left(current->left);
        if (!path.empty()) {
          Node * parent = path.top();
          if (parent->left == current) {
            parent->left = rotate_right(current);
          } else {
            parent->right = rotate_right(current);
          }
        } else {
          node = rotate_right(current);
        }
      }
    } else if (balance < -1) {  // Caso derecha
      if (elem.color > current->right->color) {
        // Caso 2: Desbalanceo derecha-derecha
        if (!path.empty()) {
          Node * parent = path.top();
          if (parent->right == current) {
            parent->right = rotate_left(current);
          } else {
            parent->left = rotate_left(current);
          }
        } else {
          node = rotate_left(current);
        }
      } else {
        // Caso 4: Desbalanceo derecha-izquierda
        current->right = rotate_right(current->right);
        if (!path.empty()) {
          Node * parent = path.top();
          if (parent->right == current) {
            parent->right = rotate_left(current);
          } else {
            parent->left = rotate_left(current);
          }
        } else {
          node = rotate_left(current);
        }
      }
    }
  }

  return node;
}

element AVLTree::search(unsigned long const color) const {
  Node const * current = root;

  while (current != nullptr) {
    if (color == current->color) {
      return element{.color = current->color, .index = current->index};
    }
    if (color < current->color) {
      current = current->left;
    } else {
      current = current->right;
    }
  }
  std::cout << "Element not found\n";
  return element{.color = 0, .index = 0};
}

int AVLTree::insert(element const elem) {
  Node * node = gsl::owner<Node *>(insert(root, elem));
  if (node == nullptr) { return -1; }   
  root = node;
  return 0;
}

void AVLTree::print() {
  // Impresión del arbol en anchura
  if (root == nullptr) { return; }

  std::queue<Node *> que;
  que.push(root);

  while (!que.empty()) {
    Node const * current = que.front();
    que.pop();

    std::cout << "Color: " << current->color << " Index: " << current->index << "\n";

    if (current->left != nullptr) { que.push(current->left); }
    if (current->right != nullptr) { que.push(current->right); }
  }
}
