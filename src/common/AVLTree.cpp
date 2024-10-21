//
// Created by beto on 21/10/24.
//

#include "AVLTree.hpp"

#include <gsl/gsl>
#include <stack>

int AVLTree::height(Node const * node) {
  return node == nullptr ? 0 : node->height;
}

int AVLTree::max(int const a, int const b) {
  return a > b ? a : b;
}

Node * AVLTree::rotate_left(Node * x) {
  Node * y  = x->right;
  Node * T2 = y->left;

  y->left  = x;
  x->right = T2;

  x->height = max(height(x->left), height(x->right)) + 1;
  y->height = max(height(y->left), height(y->right)) + 1;

  return y;
}

Node * AVLTree::rotate_right(Node * y) {
  Node * x  = y->left;
  Node * T2 = x->right;

  x->right = y;
  y->left  = T2;

  y->height = max(height(y->left), height(y->right)) + 1;
  x->height = max(height(x->left), height(x->right)) + 1;

  return x;
}

int AVLTree::get_balance(Node const * node) {
  return node == nullptr ? 0 : height(node->left) - height(node->right);
}

gsl::owner<Node*> AVLTree::insert(Node * node, element const elem) {
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
      return node;
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
          root = rotate_right(current);  // Caso raíz
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
          root = rotate_right(current);  // Caso raíz
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
          root = rotate_left(current);  // Caso raíz
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
          root = rotate_left(current);  // Caso raíz
        }
      }
    }
  }
  return node;
}