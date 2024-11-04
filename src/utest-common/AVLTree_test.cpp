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

//Arbol 1 izq 2 der pasa a 2 izq 1 der

TEST_F(AVLTreeTest, RotacionIzquierda1) {
  constexpr element elem1 = {.color = 0x00FF00, .index = 1};
  constexpr element elem2 = {.color = 0x000000, .index = 2};
  constexpr element elem3 = {.color = 0xFF0000, .index=3};
  constexpr element elem4 = {.color = 0xFFFFFF, .index=4};

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

//Arbol 2 izq 1 der pasa a 3 izq 0 der

TEST_F(AVLTreeTest, RotacionIzquierda2) {
  constexpr element elem1 = {.color = 0xFFFFFF, .index = 1};
  constexpr element elem2 = {.color = 0xFF0000, .index = 2};
  constexpr element elem3 = {.color = 0x00FF00, .index=3};
  constexpr element elem4 = {.color = 0x000000, .index=4};

  tree.insert(elem1);
  tree.insert(elem2);
  tree.insert(elem3);
  tree.insert(elem4);

  Node * root       = tree.getRoot();
  Node const * left  = root->left;
  Node const * right = root->right;

  EXPECT_EQ(tree.getHeight(root), 3);
  EXPECT_EQ(tree.getHeight(left), 2);
  EXPECT_EQ(tree.getHeight(right), 1);

  Node const * newRoot = AVLTree::get_rotate_left(root);
  EXPECT_EQ(tree.getHeight(newRoot), 4);
  EXPECT_EQ(tree.getHeight(newRoot->left), 3);
  EXPECT_EQ(tree.getHeight(newRoot->right), 0);
}

//Arbol 2 izq 1 der pasa a 1 izq 2 der

TEST_F(AVLTreeTest, RotacionDerecha1) {
  constexpr element elem1 = {.color = 0xFF0000, .index = 1};
  constexpr element elem2 = {.color = 0xFFFFFF, .index = 2};
  constexpr element elem3 = {.color = 0x00FF00, .index=3};
  constexpr element elem4 = {.color = 0x000000, .index=4};

  tree.insert(elem1);
  tree.insert(elem2);
  tree.insert(elem3);
  tree.insert(elem4);

  Node * root       = tree.getRoot();
  Node const * left  = root->left;
  Node const * right = root->right;

  EXPECT_EQ(tree.getHeight(root), 3);
  EXPECT_EQ(tree.getHeight(left), 2);
  EXPECT_EQ(tree.getHeight(right), 1);#include "common/mtdata.hpp"

  Node const * newRoot = AVLTree::get_rotate_right(root);
  EXPECT_EQ(tree.getHeight(newRoot), 3);
  EXPECT_EQ(tree.getHeight(newRoot->left), 1);
  EXPECT_EQ(tree.getHeight(newRoot->right), 2);
}

//Arbol 1 izq 2 der pasa a 0 izq y 3 der

TEST_F(AVLTreeTest, RotacionDerecha2) {
  constexpr element elem1 = {.color = 0x00FF00, .index = 1};
  constexpr element elem2 = {.color = 0x000000, .index = 2};
  constexpr element elem3 = {.color = 0xFF0000, .index=3};
  constexpr element elem4 = {.color = 0xFFFFFF, .index=4};

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

  Node const * newRoot = AVLTree::get_rotate_right(root);
  EXPECT_EQ(tree.getHeight(newRoot), 4);
  EXPECT_EQ(tree.getHeight(newRoot->left), 0);
  EXPECT_EQ(tree.getHeight(newRoot->right), 3);
}

//Arbol 1 izq 2 der

TEST_F(AVLTreeTest, BalanceArbol1) {
  constexpr element elem1 = {.color = 0x00FF00, .index = 1};
  constexpr element elem2 = {.color = 0x000000, .index = 2};
  constexpr element elem3 = {.color = 0xFF0000, .index=3};
  constexpr element elem4 = {.color = 0xFFFFFF, .index=4};

  tree.insert(elem1);
  tree.insert(elem2);
  tree.insert(elem3);
  tree.insert(elem4);

  Node const * root = tree.getRoot();
  EXPECT_EQ(tree.get_get_balance(root), -1);
}

//Arbol 2 izq 1 der

TEST_F(AVLTreeTest, BalanceArbol2) {
  constexpr element elem1 = {.color = 0xFF0000, .index = 1};
  constexpr element elem2 = {.color = 0xFFFFFF, .index = 2};
  constexpr element elem3 = {.color = 0x00FF00, .index=3};
  constexpr element elem4 = {.color = 0x000000, .index=4};

  tree.insert(elem1);
  tree.insert(elem2);
  tree.insert(elem3);
  tree.insert(elem4);

  Node const * root = tree.getRoot();
  EXPECT_EQ(tree.get_get_balance(root), 1);
}

//Arbol 3 izq 0 der

TEST_F(AVLTreeTest, BalanceArbol3) {
  constexpr element elem1 = {.color = 0xFF0000, .index = 1};
  constexpr element elem2 = {.color = 0x000000, .index = 2};
  constexpr element elem3 = {.color = 0x00FF00, .index=3};
  constexpr element elem4 = {.color = 0xFFFFFF, .index=4};

  tree.insert(elem1);
  tree.insert(elem2);
  tree.insert(elem3);
  tree.insert(elem4);

  Node const * root = tree.getRoot();
  EXPECT_EQ(tree.get_get_balance(root), -1);
}

TEST_F(AVLTreeTest, InsertArbolVacio) {
  constexpr element elem1 = {.color = 0x000000, .index = 1};
  EXPECT_EQ(tree.insert(elem1), 0);
}

TEST_F(AVLTreeTest,InsertNodoYaexistente) {
  constexpr element elem1 = {.color = 0x000000, .index = 1};
  tree.insert(elem1);
  EXPECT_EQ(tree.insert(elem1), -1);
}

TEST_F(AVLTreeTest, InsertVariosNodos) {
  constexpr element elem1 = {.color = 0x000000, .index = 1};
  constexpr element elem2 = {.color = 0xFFFFFF, .index = 2};
  constexpr element elem3 = {.color = 0xFF0000, .index = 3};

  EXPECT_EQ(tree.insert(elem1), 0);
  EXPECT_EQ(tree.insert(elem2), 0);
  EXPECT_EQ(tree.insert(elem3), 0);
}

TEST_F(AVLTreeTest, BusquedaNodoExistente) {
  constexpr element elem1 = {.color = 0xF00000, .index = 1};
  tree.insert(elem1);
  element const result = tree.search(0xF00000);
  EXPECT_EQ(result.color, elem1.color);
  EXPECT_EQ(result.index, elem1.index);
}

TEST_F(AVLTreeTest, BusquedaNodoExistente_Test) {
  constexpr element elem1 = {.color = 0x000000, .index = 1};
  tree.insert(elem1);
  element const result = tree.search(0x000000);
  EXPECT_EQ(result.color, elem1.color);
  EXPECT_EQ(result.index, elem1.index);
}

TEST_F(AVLTreeTest, ImprimirArbol2Nodos) {
  element const elem1 = {.color = 0x000000, .index = 1};
  element const elem2 = {.color = 0xFFFFFF, .index = 2};
  tree.insert(elem1);
  tree.insert(elem2);

  testing::internal::CaptureStdout();
  tree.print();
  std::string const output = testing::internal::GetCapturedStdout();

  std::string const expected_output = "Color: 0 Index: 1\nColor: 16777215 Index: 2\n";
  EXPECT_EQ(output, expected_output);
}

TEST_F(AVLTreeTest, ImprimirArbolVacio) {
  testing::internal::CaptureStdout();
  tree.print();
  std::string const output = testing::internal::GetCapturedStdout();

  std::string const expected_output;
  EXPECT_EQ(output, expected_output);
}