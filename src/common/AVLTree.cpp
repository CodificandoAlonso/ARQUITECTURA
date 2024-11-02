//
// Created by beto on 21/10/24.
//

#include "AVLTree.hpp"

#include <array>
#include <cmath>
#include <gsl/gsl>
#include <iostream>
#include <queue>
#include <stack>

int AVLTree::height(Node const * node) {
  return node == nullptr ? 0 : node->height;
}

int AVLTree::max(int const param_a, int const param_b) {
  return param_a > param_b ? param_a : param_b;
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

  std::stack<Node *> path;
  Node * insertedNode = insertNode(node, elem, path);
  if (insertedNode == nullptr) {
    return nullptr;  // Elemento ya existe en el árbol
  }

  return rebalanceTree(node, path, elem);
}

// Función para insertar el nodo y registrar el camino de inserción
Node * AVLTree::insertNode(Node * node, element const elem, std::stack<Node *> & path) {
  Node * current = node;
  Node * parent  = nullptr;

  while (current != nullptr) {
    parent = current;
    path.push(parent);
    if (elem.color < current->color) {
      current = current->left;
    } else if (elem.color > current->color) {
      current = current->right;
    } else {
      return nullptr;  // Elemento duplicado, no se inserta
    }
  }

  if (elem.color < parent->color) {
    parent->left = static_cast<gsl::owner<Node *>>(new Node(elem));
  } else {
    parent->right = static_cast<gsl::owner<Node *>>(new Node(elem));
  }

  return parent;
}

// Función para re-balancear el árbol después de la inserción
gsl::owner<Node *> AVLTree::rebalanceTree(Node * node, std::stack<Node *> & path,
                                          element const elem) {
  while (!path.empty()) {
    Node * current = path.top();
    path.pop();

    current->height = 1 + max(height(current->left), height(current->right));

    int const balance = get_balance(current);

    if (balance > 1 || balance < -1) { node = handleImbalance(node, current, path, elem); }
  }

  return node;
}

// Función para manejar los cuatro tipos de desbalance (LL, LR, RR, RL)
Node * AVLTree::handleImbalance(Node * root, Node * current, std::stack<Node *> & path,
                                element const elem) {
  int const balance                 = get_balance(current);
  std::array<Node *, 2> const nodes = {root, current};

  if (balance > 1) {
    if (elem.color < current->left->color) {
      return rotateWithParent(nodes, path, rotate_right(current));
    }
    current->left = rotate_left(current->left);
    return rotateWithParent(nodes, path, rotate_right(current));
  }
  if (balance < -1) {
    if (elem.color > current->right->color) {
      return rotateWithParent(nodes, path, rotate_left(current));
    }
    current->right = rotate_right(current->right);
    return rotateWithParent(nodes, path, rotate_left(current));
  }

  return root;
}

// Función auxiliar para realizar la rotación en el nodo padre
Node * AVLTree::rotateWithParent(std::array<Node *, 2> nodes, std::stack<Node *> & path,
                                 Node * newSubRoot) {
  Node *& root   = nodes[0];
  Node * current = nodes[1];

  if (!path.empty()) {
    Node * parent = path.top();
    if (parent->left == current) {
      parent->left = newSubRoot;
    } else {
      parent->right = newSubRoot;
    }
  } else {
    root = newSubRoot;
  }

  return root;
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
  gsl::owner<Node *> node = insert(root, elem);
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
