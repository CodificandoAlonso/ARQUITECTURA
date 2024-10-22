//
// Created by beto on 21/10/24.
//

#include "AVLTree.hpp"

#include <gsl/gsl>
#include <iostream>
#include <cmath>
#include <queue>
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

element AVLTree::search(unsigned long const color) {
  Node const * current = root;

  while (current != nullptr) {
    if (color == current->color) {
      return element{.color=current->color, .index=current->index};
    } else if (color < current->color) {
      current = current->left;
    } else {
      current = current->right;
    }
  }
  std::cout << "Element not found\n";
  return element{.color=0, .index=0};
}



int AVLTree::insert(element const elem) {
  Node * node = static_cast<gsl::owner<Node *>>(insert(root, elem));
  if (node == nullptr) { return -1; }
  root = node;
  return 0;
}

void AVLTree::print() {
  // Impresión del arbol en anchura
  if (root == nullptr) { return; }

  std::queue<Node *> q;
  q.push(root);

  while (!q.empty()) {
    Node * current = q.front();
    q.pop();

    std::cout << "Color: " << current->color << " Index: " << current->index << "\n";

    if (current->left != nullptr) { q.push(current->left); }
    if (current->right != nullptr) { q.push(current->right); }
  }
}
