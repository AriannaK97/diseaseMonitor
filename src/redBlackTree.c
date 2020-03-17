//
// Created by AriannaK97 on 14/3/20.
//
#include "../header/redBlackTree.h"
#include <stdio.h>
#include "../header/list_lib.h"

void leftRotate(rbTree* tree, rbNode* x){
    rbNode* y = x->right;

    x->right = y->left;
    if(x->right != tree->nil)
        x->right->parent = x;

    y->parent = x->parent;
    if(y->parent == tree->nil){
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

    x->left = y->right;
    if(x->left != tree->nil)
        x->left->parent = x;

    y->parent = x->parent;

    if(y->parent == tree->nil){
        tree->root = y;
    }else if(x == x->parent->left){
        x->parent->left = y;
    }else{
        x->parent->right = y;
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
        return false;
    }else if(date1 != NULL && date2 == NULL){
        return true;
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

int compare_dates (Date* d1, Date* d2)
{
    if(d1 == NULL || d2 == NULL)
        return 3;

    if (d1->year < d2->year)
        return -1;

    else if (d1->year > d2->year)
        return 1;

    if (d1->year == d2->year)
    {
        if (d1->month<d2->month)
            return -1;
        else if (d1->month>d2->month)
            return 1;
        else{
            if (d1->day<d2->day)
                return -1;
            else if(d1->day>d2->day)
                return 1;
            else
                return 0;
        }
    }
}

rbTree* createRbTree(){
    rbTree* tree = malloc(sizeof(rbTree));
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
    Date* treeDate;
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

    z->left = tree->nil;
    z->right = tree->nil;

    while (x != tree->nil){
        y = x;
        if(compare_dates(z->key, x->key) == -1)
            x = x->left;
        else
            x = x->right;
/*        else if(compare_dates(z->key, x->key) == 1)
            x = x->right;
        else if(compare_dates(z->key, x->key) == 0)
            x = x->right;
        else
            return;*/
    }

    z->parent = y;

    if(y == tree->nil)
        tree->root = z;
    else if(compare_dates(z->key, y->key) == -1)
        y->left = z;
    else
        y->right = z;


    z->colour = Red;
    rbInsertFixup(tree, z);
}

void rbInsertFixup(rbTree* tree, rbNode* z){
    rbNode* y;
    while(z->parent->colour == Red){
        if(z->parent == z->parent->parent->left){
            y = z->parent->parent->right;
            if(y->colour == Red){
                z->parent->colour = Black;
                y->colour = Black;
                z->parent->parent->colour = Red;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->right){
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->colour = Black;
                z->parent->parent->colour = Red;
                rightRotate(tree, z->parent->parent);
            }
        }
        else{
            y = z->parent->parent->left;
            if(y->colour == Red){
                z->parent->colour = Black;
                y->colour = Black;
                z->parent->parent->colour = Red;
                z = z->parent->parent;
            }
            else{
                if(z == z->parent->left){
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->colour = Black;
                z->parent->parent->colour = Red;
                leftRotate(tree, z->parent->parent);
            }
            if(z == tree->root){
                break;
            }
        }

    }
    tree->root->colour = Black;
}


void printRbTree(rbNode* root, int depth){

    if(root->key == NULL){
        return;
    }
    printRbTree(root->left, depth+1);
    PatientCase* patient;
    patient = root->listNodeEntry->item;
    fprintf(stdout,"depth = %d, case number: %d | name: %s | surname: %s | virus: %s | country: %s | importDate: %d-%d-%d | "
                   "exportDate: %d-%d-%d\n",depth, patient->caseNum, patient->name, patient->surname, patient->virus,
            patient->country, patient->entryDate->day, patient->entryDate->month, patient->entryDate->year,
            patient->exitDate->day, patient->exitDate->month, patient->exitDate->year);
    printRbTree(root->right, depth+1);

}

rbNode* searchRbNodeRec(rbNode* root, rbNode* nil, void* key){
    if(root == NULL || root == nil){
        return NULL;
    }

    if(compare_dates(root->key, key) == 1){
        searchRbNodeRec(root->left, nil, key);
    }else if(compare_dates(root->key, key) == -1){
        searchRbNodeRec(root->right, nil, key);
    } else {
        searchRbNodeRec(root->left, nil, key);
        PatientCase* patient = root->listNodeEntry->item;
        fprintf(stdout,
                "case number: %d | name: %s | surname: %s | virus: %s | country: %s | importDate: %d-%d-%d | "
                "exportDate: %d-%d-%d\n", patient->caseNum, patient->name, patient->surname, patient->virus,
                patient->country, patient->entryDate->day, patient->entryDate->month, patient->entryDate->year,
                patient->exitDate->day, patient->exitDate->month, patient->exitDate->year);
        searchRbNodeRec(root->right, nil, key);
    }
    return root;

}

rbNode* searchRbNode(rbTree* tree, void* key){
    return searchRbNodeRec(tree->root, tree->nil, key);
}
