//
// Created by AriannaK97 on 19/3/20.
//

#include <stdio.h>
#include <string.h>
#include "../header/binaryMaxHeap.h"
#include "../header/hashTable.h"

Heap* createHeap(){
    Heap* heapTree = malloc(sizeof(Heap));
    heapTree->root = NULL;
    return heapTree;
}

HeapNode* createHeapNode(char* data, int dataSum){
    HeapNode* heapNode =  malloc(sizeof(HeapNode));
    heapNode->data = malloc(sizeof(char)*32);
    heapNode->right = NULL;
    heapNode->left = NULL;
    heapNode->parent = NULL;
    heapNode->dataSum = dataSum;
    strcpy(heapNode->data, data);
    return heapNode;
}

int minDepth(HeapNode *root){
    if (root == NULL)
        return 0;

    int lHeight = minDepth(root->left);
    int rHeight = minDepth(root->right);

    return MIN(lHeight, rHeight)+1;
}

int maxDepth(HeapNode* root){
    if(root == NULL){
        return 0;
    }
    int leftDepth =  maxDepth((HeapNode*)root->left);
    int rightDepth = maxDepth((HeapNode*)root->right);
    if(leftDepth > rightDepth){
        return leftDepth+1;
    }
    return rightDepth+1;
}
Heap* getSubHeapTree(Heap* primalTree, HeapNode* newRoot){

    Heap* subHeapTree = malloc(sizeof(Heap));
    subHeapTree->root = newRoot;
    return subHeapTree;
}

void swapValues(HeapNode** node1, HeapNode** node2){
    char *temp1 = malloc(DATA_SPACE* sizeof(char));
    int sum1;

    strcpy(temp1, (*node1)->data);
    sum1 = (*node1)->dataSum;
    strcpy((*node1)->data, (*node2)->data);
    (*node1)->dataSum = (*node2)->dataSum;
    strcpy((*node2)->data, temp1);
    (*node2)->dataSum = sum1;

}

HeapNode* insertHeap(Heap* heapTree, HeapNode* newNode){

    HeapNode* root = heapTree->root;
    int hMinLeft = 0;
    int hMinRight = 0;


    if (root == NULL){
        root = newNode;
        //heapTree->lastInserted = newNode;
        return root;
    }

    hMinLeft = minDepth(root->left);
    hMinRight = minDepth(root->right);

    if(hMinLeft <= hMinRight){

        Heap* subTree = getSubHeapTree(heapTree, root->left);
        root->left = insertHeap(subTree, newNode);
        root->left->parent = root;
        freeHeapTree(subTree);

        root->left = root->left;
        if(root->left != NULL && root->left->dataSum > root->left->parent->dataSum){
            swapValues(&root->left, &root->left->parent);
        }

    }else if(hMinLeft > hMinRight){
        Heap* subTree = getSubHeapTree(heapTree, root->right);
        root->right= insertHeap(subTree, newNode);
        root->right->parent = root;
        if(root->right != NULL && root->right->dataSum > root->right->parent->dataSum){
            swapValues(&root->right, &root->right->parent);
        }

    }

    return root;
}

HeapNode* getLeft(HeapNode* A){
    if (A == NULL){
        return NULL;
    }
    HeapNode* newIndex;
    if((newIndex = (HeapNode*)A->left) != NULL)
        return newIndex;
    return NULL;
}

HeapNode* getRight(HeapNode* A){
    if (A == NULL){
        return NULL;
    }
    HeapNode* newIndex;
    if((newIndex = (HeapNode*)A->right) != NULL)
        return newIndex;
    return NULL;
}

HeapNode* getParent(HeapNode* A){
    HeapNode* newIndex;
    if((newIndex = (HeapNode*)A->parent) != NULL)
        return newIndex;
    return NULL;
}

void exchange(HeapNode** A, HeapNode** i){
    HeapNode* temp = *A;
    *A = *i;
    *i = temp;
}

/*maxHeapify from CLRS*/
/*
void maxHeapify(HeapNode* A){
    HeapNode* left;
    HeapNode* right;
    HeapNode* largest;
    largest = A;
    left = getLeft(A);
    right = getRight(A);

    if(left != NULL){
        if(left->dataSum > largest->dataSum){
            largest = left;
        }
    }else {
        largest = A;
    }
    if(right != NULL){
        if(right->dataSum > largest->dataSum){
            largest = right;
        }
    }
    if(largest != A){
        exchange(&A, &largest);
        maxHeapify(A);
    }
}
*/

void printGivenLevel(HeapNode* root, int level, int *k) {
    if (root == NULL /*|| k < 0*/)
        return;
    if (level == 1 && *k > 0){
        *k -= 1;
        printf("%s: %d\n", root->data, root->dataSum);
    }else if (level > 1){
        printGivenLevel((HeapNode*)root->left, level-1, k);
        printGivenLevel((HeapNode*)root->right, level-1, k);
    }
}

void printLevelOrder(HeapNode* root, int k){
    int h = maxDepth(root);
    int i;
    if(k > h){
        k = h;
    }
    for (i = 1; i <= h; i++){
        printGivenLevel(root, i, &k);
    }
}

HeapNode* ifNodeExists(HeapNode* node, char* key){
    HeapNode* res1;
    HeapNode* res2;
    if (node == NULL || node->data == NULL)
        return NULL;

    if (strcmp(node->data, key)==0)
        return node;

    /* then recur on left sutree */
    res1 = ifNodeExists((HeapNode*)node->left, key);

    if(res1 != NULL) return node; // node found, no need to look further

    /* node is not found in left, so recur on right subtree */
    res2 = ifNodeExists((HeapNode*)node->right, key);

    return res2;
}

void freeNode(HeapNode* node){
    free(node->data);
    free(node);
}

void freeHeapNode(Heap* tree, HeapNode* node){
    if(node != NULL){
        freeHeapNode(tree, node->left);
        freeHeapNode(tree ,node->right);
        freeNode(node);
    }
}

void freeHeapTree(Heap* heapTree){
    freeHeapNode(heapTree, heapTree->root);
    free(heapTree);
}



