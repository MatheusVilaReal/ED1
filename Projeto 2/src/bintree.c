#include <stdio.h>          //printf
#include <stdlib.h>         //malloc
#include <math.h>
#include <float.h>

#include "shapes.h"

#include "bintree.h"

#define SPACING 10

typedef struct Node{

    Data data;

    struct Node* parent;
    struct Node* left, *right;

    unsigned long index;

} Node_t;

typedef struct Tree{

    Node_t* root;

    unsigned long size;

} Tree_t;

static void wrapped_insertNode(Node_t* parent, Node_t* node, bool (*filter)(Node, Node, unsigned int, double (*getX)(Shape), double (*getY)(Shape)), double (*getX)(Shape), double (*getY)(Shape)){

    if(filter(parent, node, getNodeLevel(parent) + 1, getX, getY)){

        puts("AA");

        if(!parent->right){

            node->parent = parent;
            parent->right = node;

        } else
            wrapped_insertNode(parent->right, node, filter, getX, getY);

    } else{

        puts("BB");

        if(!parent->left){

            node->parent = parent;
            parent->left = node;
        
        } else
            wrapped_insertNode(parent->left, node, filter, getX, getY);
    }
}

static Node_t* wrapped_findMinNode(Node_t* node, Node_t* sub, long depth, double smallest, double (*getX)(Shape), double (*getY)(Shape)){

    if(node == NULL)
        return sub;

    if(depth & 1){

        if(getY(getNodeData(node)) < smallest){
            
            smallest = getY(getNodeData(node));
            sub = node;
        }
    
    } else{

        if(getX(getNodeData(node)) < smallest){
            
            smallest = getX(getNodeData(node));
            sub = node;
        }
    }

    Node_t* sub_2 = wrapped_findMinNode(getLeftChild(node), sub, depth, smallest, getX, getY);
    sub_2 = wrapped_findMinNode(getRightChild(node), sub_2, depth, smallest, getX, getY);

    return sub_2;
}

static Node_t* wrapped_kd_buildTree(Node* array, long start, long end, long depth, unsigned long index, Node_t* parent, int (*compare_X)(const void*, const void*), int (*compare_Y)(const void*, const void*)){

    if (start > end)
        return NULL;

    if(depth & 1)
        qsort(array, (end - start) + 1, sizeof(Node), compare_Y);
    else
        qsort(array, (end - start) + 1, sizeof(Node), compare_X);

    // for(int i = 0; i < 7; i++){

    //     printRect(getNodeData(array[i]));
    // }

    // printf("\n\n");

    long median = (start + end)/2;

    // printf("%ld\n", start + end);

    Node_t* root = array[median];

    root->parent = parent;
    root->index = index;
  
    root->right = wrapped_kd_buildTree(array, start, median - 1, depth + 1, index + 1, root, compare_X, compare_Y);
    root->left = wrapped_kd_buildTree(array, median + 1, end, depth, depth + 1, root, compare_X, compare_Y);

    return root;
}

static Node_t* findMinNode(Node_t* node, long depth, double (*getX)(Shape), double (*getY)(Shape)){

    return wrapped_findMinNode(node, NULL, depth, DBL_MAX, getX, getY);
}

static Node_t* closest(double x, double y, Node node_1, Node node_2, double (*getX)(Shape), double (*getY)(Shape)){

    if(hypot(x - getX(getNodeData(node_1)), y - getY(getNodeData(node_1))) < hypot(x - getX(getNodeData(node_2)), y - getY(getNodeData(node_2))))
        return node_1;
    else
        return node_2;
}

Tree newTree(){

    Tree_t* tree = (Tree_t*) malloc(sizeof(Tree_t));

    tree->size = 0;
    tree->root = NULL;

    return tree;
}

Node newNode(Data data){

    Node_t* node = (Node_t*) malloc(sizeof(Node_t));

    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    node->data = data;

    node->index = 0;

    return (Node) node;
}

Node getRoot(Tree tree){

    Tree_t* toGet = (Tree_t*) tree;

    return (Node) toGet->root;
}

Node getNodeParent(Node node){

    if(node == NULL)
        return NULL;

    Node_t* toGetParent = (Node_t*) node;

    return (Node) toGetParent->parent;
}

Node getLeftChild(Node node){

    if(node == NULL)
        return NULL;

    Node_t* toGetLeft = (Node_t*) node;

    return (Node) toGetLeft->left;    
}

Node getRightChild(Node node){

    if(node == NULL)
        return NULL;

    Node_t* toGetRight = (Node_t*) node;

    return (Node) toGetRight->right;    
}

Node searchNearest(Node node, double x, double y, unsigned int depth, double (*getX)(Shape), double (*getY)(Shape)){

    if(node == NULL)
        return NULL;

    Node nextBranch, otherBranch;

    if(depth & 1){

        if(y < getY(getNodeData(node))){

            nextBranch = getLeftChild(node);
            otherBranch = getRightChild(node);

        } else{

            nextBranch = getRightChild(node);
            otherBranch = getLeftChild(node);
        }

    } else{

        if(x < getX(getNodeData(node))){

            nextBranch = getLeftChild(node);
            otherBranch = getRightChild(node);
            
        } else{

            nextBranch = getRightChild(node);
            otherBranch = getLeftChild(node);
        }
    }

    Node temp = searchNearest(nextBranch, x, y, depth + 1, getX, getY);
    Node best = closest(x, y, temp, node, getX, getY);

    double radius = hypot(x - getX(getNodeData(best)), y - getY(getNodeData(best))), distAxis;

    if(depth & 1)
        distAxis = y - getY(getNodeData(node));
    else
        distAxis = x - getX(getNodeData(node));
    
    if(radius >= distAxis){

        temp = searchNearest(otherBranch, x, y, depth + 1, getX, getY);
        best = closest(x, y, temp, best, getX, getY);
    }

    return best;
}

Node searchBranch(Node node, unsigned long index){

    if(node == NULL || getNodeIndex(node) == index)
        return node;

    Node node_2 = searchBranch(getLeftChild(node), index);

    if(node_2 == NULL)
        node_2 = searchBranch(getRightChild(node), index);

    return node_2;
}

Node searchTree(Tree tree, unsigned long index){

    Node_t* node = searchBranch(getRoot(tree), index);

    if(node == NULL)
        return 0;
    else
        return node;
}

Data getNodeData(Node node){

    if(node == NULL)
        return NULL;

    Node_t* toGetData = (Node_t*) node;

    return (Data) toGetData->data;
}

unsigned long getNodeIndex(Node node){

    if(node == NULL)
        return 0;

    Node_t* toGetIndex = (Node_t*) node;

    return toGetIndex->index;
}

unsigned long getTreeSize(Tree tree){

    Tree_t* toGetSize = (Tree_t*) tree;

    return toGetSize->size;
}

unsigned long insertNode(Tree tree, Data data, unsigned long parent, bool (*filter)(Node, Node, unsigned int, double (*getX)(Shape), double (*getY)(Shape)), double (*getX)(Shape), double (*getY)(Shape)){

    if(data == NULL)
        return 0;

    Tree_t* receiver = (Tree_t*) tree;

    Node_t* node = newNode(data);
    node->index = receiver->size++;

    Node_t* parentNode = (Node_t*) searchTree(tree, parent);

    if(receiver->size == 1)
        receiver->root = node;
    else
        wrapped_insertNode(parentNode, node, filter, getX, getY);

    return node->index;
}

unsigned short getNodeLevel(Node node){

    Node_t* toFind = (Node_t*) node;

    unsigned short level = 0;

    while(toFind != NULL){

        ++level;

        toFind = toFind->parent;
    }

    return level;
}

bool kd_filter(Node parent, Node toInsert, unsigned int depth, double (*getX)(Shape), double (*getY)(Shape)){

    if(depth & 1)
        return (getY(getNodeData(parent)) < getY(getNodeData(toInsert)));
    else
        return (getX(getNodeData(parent)) < getX(getNodeData(toInsert)));
}

bool isLeaf(Node node){

    if(node == NULL)
        return 1;

    Node_t* toLeaf = (Node_t*) node;

    return !(toLeaf->left && toLeaf->right);
}

void kd_buildTree(Tree tree, Node* array, long size, int (*compare_X)(const void*, const void*), int (*compare_Y)(const void*, const void*)){

    Tree_t* toBuild = (Tree_t*) tree;

    toBuild->size = size;

    toBuild->root = wrapped_kd_buildTree(array, 0, size - 1, 0, 0, NULL, compare_X, compare_Y);
}

void kd_rmNode(Tree tree, Node node, double (*getX)(Shape), double (*getY)(Shape), void (*free_shape)(void*)){

    Node_t* toRm = (Node_t*) node, * sub;

    if(isLeaf(node)){

        if(toRm->parent->left == toRm)
            toRm->parent->left = NULL;
        else
            toRm->parent->right = NULL;

        free_shape(toRm->data);
        free(toRm);

    } else if(toRm->right != NULL){

            sub = findMinNode(toRm->right, getNodeLevel(toRm), getX, getY);

            free_shape(toRm->data);
            toRm->data = sub->data;

            kd_rmNode(tree, sub, getX, getY, free_shape);

        } else{

            sub = findMinNode(toRm->left, getNodeLevel(toRm), getX, getY);

            free_shape(toRm->data);
            toRm->data = sub->data;

            kd_rmNode(tree, sub, getX, getY, free_shape);

            toRm->right = toRm->left;
            toRm->left = NULL;
    }
}

void printBranch(Node node, int space, void (*print_data)(void*)){

    if(node == NULL)
        return;

    space += SPACING;

    printBranch(getRightChild(node), space, print_data);

    printf("\n");

    for(int i = SPACING; i < space; i++)
        printf(" ");

    print_data(getNodeData(node));

    // if(toPrint->parent)
    //     printf("1");
    // else
    //     printf("NULL");

    printBranch(getLeftChild(node), space, print_data);
}

void printTree(Tree tree, void (*print_data)(void*)){

    printBranch(getRoot(tree), 0, print_data);    
}

void chopBranch(Node node, void (*free_node)(void*)){

    if(node == NULL)
        return;

    chopBranch(getLeftChild(node), free_node);
    chopBranch(getRightChild(node), free_node);

    free_node(getNodeData(node));
    free(node);
}

void chopTree(Tree tree, void (*free_node)(void*)){

    chopBranch(getRoot(tree), free_node);

    free(tree);
}