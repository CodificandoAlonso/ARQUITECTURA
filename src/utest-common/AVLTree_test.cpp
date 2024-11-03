#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "common/AVLTree.hpp"

class AVLTreeTest : public ::testing::Test {
  protected:
  AVLTree tree;
};

TEST_F(AVLTreeTest, AlturaDeNodoNulo) {
  Node const * nullNode = nullptr;
  EXPECT_EQ(tree.getHeight(nullNode), 0);
}

TEST_F(AVLTreeTest, AlturaDe1Nodo) {
  constexpr element elem = {.color=0xFFFFFF, .index=1};
  std::unique_ptr<Node> const singleNode = std::make_unique<Node>(elem);
  EXPECT_EQ(tree.getHeight(singleNode.get()), 1);
}

TEST_F(AVLTreeTest, Alturade1ArbolCon3Nodos) {
  constexpr element elem1 = {.color = 0xFFFFFF, .index = 1};
  constexpr element elem2 = {.color = 0x000000, .index = 2};
  constexpr element elem3 = {.color=0xFF0000, .index=3};

  tree.insert(elem1);
  tree.insert(elem2);
  tree.insert(elem3);

  EXPECT_EQ(tree.getHeight(tree.getRoot()), 2);
}

TEST_F(AVLTreeTest, Alturade1ArbolCon4Nodos) {
  constexpr element elem1 = {.color = 0xFFFFFF, .index = 1};
  constexpr element elem2 = {.color = 0x000000, .index = 2};
  constexpr element elem3 = {.color=0xFF0000, .index=3};
  constexpr element elem4 = {.color=0x00FF00, .index=4};

  tree.insert(elem1);
  tree.insert(elem2);
  tree.insert(elem3);
  tree.insert(elem4);

  EXPECT_EQ(tree.getHeight(tree.getRoot()), 3);
}

TEST_F(AVLTreeTest, FuncionMax) {
  EXPECT_EQ(tree.getMax(8, 5), 8);
}

TEST_F(AVLTreeTest, RotacionIzquierda1) {
  constexpr element elem1 = {.color = 0x00FF00, .index = 1};
  constexpr element elem2 = {.color = 0x000000, .index = 2};
  constexpr element elem4 = {.color = 0xFFFFFF, .index=3};
  constexpr element elem3 = {.color = 0xFF0000, .index=4};

  tree.insert(elem1);
  tree.insert(elem2);
  tree.insert(elem3);
  tree.insert(elem4);

  Node * root       = tree.getRoot();
  Node const * left  = root->left;
  Node const * right = root->right;

  EXPECT_EQ(tree.getHeight(root), 3);
  EXPECT_EQ(tree.getHeight(left), 1);
  EXPECT_EQ(tree.getHeight(right), 2);

  Node const * newRoot = AVLTree::get_rotate_left(root);
  EXPECT_EQ(tree.getHeight(newRoot), 3);
  EXPECT_EQ(tree.getHeight(newRoot->left), 2);
  EXPECT_EQ(tree.getHeight(newRoot->right), 1);
}