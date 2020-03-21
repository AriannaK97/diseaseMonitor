//
// Created by AriannaK97 on 19/3/20.
//

#include <stdio.h>
#include <string.h>
#include "../header/binaryMaxHeap.h"

void createMaxHeap(HeapNode* A){
    for(int i = 0; i > 0; i--){
        maxHeapify(A);
    }
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

HeapNode* insertHeap(HeapNode* root, HeapNode* newNode){
    /* If the tree is empty, return a new node */
    if (root == NULL)
        return newNode;
    else if(root->data == NULL){
        root = newNode;
        return root;
    }

    /* Otherwise, recur down the tree */
    if (newNode->dataSum < root->dataSum)
        root->left  = (struct HeapNode*)insertHeap((HeapNode*)root->left, newNode);
    else if (newNode->dataSum >= root->dataSum)
        root->right = (struct HeapNode*)insertHeap((HeapNode*)root->right, newNode);

    /* return the (unchanged) node pointer */
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
void maxHeapify(HeapNode* A){
    HeapNode* left;
    HeapNode* right;
    HeapNode* largest;

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

HeapNode* createHeapNode(char* data, int dataSum){
    HeapNode* heapNode =  malloc(sizeof(HeapNode));
    heapNode->data = malloc(sizeof(char)*32);
    heapNode->right = NULL;
    heapNode->left = NULL;
    heapNode->parent = NULL;
    heapNode->dataSum = dataSum;
    heapNode->data = data;
    return heapNode;
}

void printGivenLevel(HeapNode* root, int level, int *k) {
    if (root == NULL /*|| k < 0*/)
        return;
    if (level == 1 && *k > 0){
        *k -= 1;
        printf("%s: %d\n", root->data, root->dataSum);
    }else if (level > 1){
        printGivenLevel((HeapNode*)root->right, level-1, k);
        printGivenLevel((HeapNode*)root->left, level-1, k);
    }
}

void printLevelOrder(HeapNode* root, int k){
    int h = maxDepth(root);
    int i;
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

void freeHeapTree(HeapNode* heapTree){
    if(heapTree != NULL){
        freeHeapTree(heapTree->left);
        freeHeapTree(heapTree->right);
        free(heapTree);
    }
}

