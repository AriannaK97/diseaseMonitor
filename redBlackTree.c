//
// Created by AriannaK97 on 14/3/20.
//
#include "redBlackTree.h"
#include <stdio.h>

void leftRotate(rbTree* tree, rbNode* x){
    rbNode* y = x->right;
    x->right = y->right;
    if(y->left != tree->nil)
        y->left->parent = x;
    y->parent = x->parent;
    if(x->parent == tree->nil){
        tree->root = y;
    }else if(x == x->parent->left){
        x->parent->left = y;
    }else{
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rightRotate(rbTree* tree, rbNode* x){
    rbNode* y = x->left;

    x->left = y->left;
    if(y->right != tree->nil)
        y->right->parent = x;

    y->parent = x->parent;

    if(x->parent == tree->nil){
        tree->root = y;
    }else if(x == x->parent->right){
        x->parent->right = y;
    }else{
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}
/**
 * If the first argument is greater than the second it returns true
 * else it return false.
 * */
bool compareDate(Date* date1, Date* date2){
    if(date1 == NULL && date2 != NULL){
        return date2;
    }else if(date1 != NULL && date2 == NULL){
        return date1;
    }
    if(date1->year > date2->year) {
        return  true;
    }else if(date1->month > date2->month){
        return true;
    }else if(date1->day > date2->day){
        return true;
    }else
        return false;
}

rbTree* createRbTree(){
    rbTree* tree = malloc(sizeof(rbTree));
    tree->root = malloc(sizeof(rbNode));
    tree->nil = malloc(sizeof(rbNode));

    tree->nil->parent = NULL;
    tree->nil->key = NULL;
    tree->nil->listNodeEntry = NULL;
    tree->nil->colour = Black;
    tree->nil->left = tree->nil;
    tree->nil->right = tree->nil;
    tree->root = tree->nil;

    return tree;
}


void* getKey(Node* listNode){
    Date* treeDate = malloc(sizeof(Date));
    PatientCase* patient = listNode->item;
    treeDate = patient->entryDate;
    return treeDate;
}

rbNode* createRbTreeNode(void* listNode){
    rbNode* treeNode = malloc(sizeof(rbNode));

    treeNode->key = getKey(listNode);
    treeNode->listNodeEntry = listNode;
    treeNode->colour =  Red;
    treeNode->parent = NULL;

    return treeNode;
}

void rbInsert(rbTree* tree, rbNode* z){
    rbNode* y = tree->nil;
    rbNode* x = tree->root;

    while (x != tree->nil){
        y = x;
        if(compareDate(x->key, z->key))
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;

    if(y == tree->nil)
        tree->root = z;
    else if(compareDate(x->key, z->key))
        y->left = z;
    else
        y->right = z;

    z->left = tree->nil;
    z->right = tree->nil;
    z->colour = Red;
    rbInsertFixup(tree, z);
}

void rbInsertFixup(rbTree* tree, rbNode* z){
    rbNode* y;
    while(z->parent->colour == Red){
        //printf("I am fixup insert :)\n");
        if(z->parent == z->parent->parent->left){
            y = z->parent->parent->right;
            if(y->colour == Red){
                z->parent->colour = Black;
                y->colour = Black;
                z->parent->parent->colour = Red;
                z = z->parent->parent;
            }else{
                if(z == z->parent->right){
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->colour = Black;
                z->parent->parent->colour = Red;
                rightRotate(tree, z);
            }
        } else{
            y = z->parent->parent->left;
            if(y->colour == Red){
                z->parent->colour = Black;
                y->colour = Black;
                z->parent->parent->colour = Red;
                z = z->parent->parent;
            }else{
                if(z == z->parent->left){
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->colour = Black;
                z->parent->parent->colour = Red;
                leftRotate(tree, z);
            }
        }

    }
    tree->root->colour = Black;
}

void searchRbNode(rbTree* tree){

}
