//
// Created by beto on 21/10/24.
//

#ifndef AVLTREE_HPP
#define AVLTREE_HPP

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

  public:
    AVLTree() = default;
    int insert(element elem);
    [[nodiscard]] element search(unsigned long color) const;
    void print();
};

#endif  // AVLTREE_HPP
