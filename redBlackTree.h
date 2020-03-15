//
// Created by AriannaK97 on 14/3/20.
//

#ifndef DISEASEMONITOR_REDBLACKTREE_H
#define DISEASEMONITOR_REDBLACKTREE_H

#include <stdbool.h>
#include <stdlib.h>
#include "structs.h"

typedef struct rbNode{
    int colour;
    struct Date* key;
    struct Node* listNodeEntry;
    struct rbNode* left;
    struct rbNode* right;
    struct rbNode* parent;
}rbNode;

typedef struct rbTree{
    struct rbNode* root;
    struct rbNode* nil;
}rbTree;

enum colours{
    Red,
    Black,
};

rbTree* createRbTree();
rbNode* createRbTreeNode(void* listNode);
void leftRotate(rbTree* rbTree, rbNode* treeNode);
void rbInsert(rbTree* tree, rbNode* z);
void rbInsertFixup(rbTree* tree, rbNode* z);
void* getKey(Node* listNode);
bool compareDate(Date* date1, Date* date2);
rbNode* searchRbNode(rbTree* tree, void* key);

#endif //DISEASEMONITOR_REDBLACKTREE_H
