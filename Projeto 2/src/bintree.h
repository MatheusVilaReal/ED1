#ifndef BIN_TREE_H
#define BIN_TREE_H

#include "types.h"

Tree newTree();

Node newNode(Data data);

Node getRoot(Tree tree);

Node getNodeParent(Node node);

Node getLeftChild(Node node);

Node getRightChild(Node node);

Node searchBranch(Node node, unsigned long index);

Node searchTree(Tree tree, unsigned long index);

Node searchNearest(Node node, double x, double y, unsigned int depth, double (*getX)(Shape), double (*getY)(Shape));

Data getNodeData(Node node);

unsigned long getNodeIndex(Node node);

unsigned long getTreeSize(Tree tree);

unsigned long insertNode(Tree tree, Data data, unsigned long parent, bool (*filter)(Node, Node, unsigned int, double (*getX)(Shape), double (*getY)(Shape)), double (*getX)(Shape), double (*getY)(Shape));

unsigned short getNodeLevel(Node node);

bool isLeaf(Node node);

bool kd_filter(Node parent, Node toInsert, unsigned int depth, double (*getX)(Shape), double (*getY)(Shape));

void kd_buildTree(Tree tree, Node* array, long size, int (*compare_X)(const void*, const void*), int (*compare_Y)(const void*, const void*));

void kd_rmNode(Tree tree, Node node, double (*getX)(Shape), double (*getY)(Shape), void (*free_node)(void*));

void printBranch(Node node, int space, void (*print_data)(void*));

void printTree(Tree tree, void (*print_data)(void*));

void chopBranch(Node node, void (*free_node)(void*));

void chopTree(Tree tree, void (*free_node)(void*));

#endif