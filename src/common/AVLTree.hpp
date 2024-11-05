//
// Created by beto on 21/10/24.
//

#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <gsl/gsl>
#include <stack>

struct element {
    unsigned long color;
    unsigned int index;
};

class Node {
  public:
    unsigned long color;  // rgb concatenados en un solo int
    unsigned int index;
    Node * left  = nullptr;
    Node * right = nullptr;
    int height{1};

    explicit Node(element const elem) : color(elem.color), index(elem.index) { }
};

class AVLTree {
  private:
    Node * root = nullptr;

    static int height(Node const * node);
    static int max(int param_a, int param_b);
    static Node * rotate_right(Node * param_y);
    static Node * rotate_left(Node * param_x);
    static int get_balance(Node const * node);
    static Node * insert(Node * node, element elem);
    static Node * rotateWithParent(std::array<Node *, 2> nodes, std::stack<Node *> & path,
                                     Node * newSubRoot);
    static Node * handleImbalance(Node * root, Node * current, std::stack<Node *> & path,
                                           element elem);
    static gsl::owner<Node *> rebalanceTree(Node * node, std::stack<Node *> & path,
                                                     element elem);
    static Node * insertNode(Node * node, element elem, std::stack<Node *> & path);

  public:
    AVLTree() = default;
    int insert(element elem);
    [[nodiscard]] element search(unsigned long color) const;
    void print();
    static int getHeight(Node const * node) ;
    [[nodiscard]] Node* getRoot() const { return root; }
    static int getMax(int param_a, int param_b);
    static Node * get_rotate_left(Node * param_x);
    static Node * get_rotate_right(Node * param_x);
    static int get_get_balance(Node const * node);
};

#endif  // AVLTREE_HPP
