//
// Created by AriannaK97 on 19/3/20.
//

#ifndef DISEASEMONITOR_BINARYMAXHEAP_H
#define DISEASEMONITOR_BINARYMAXHEAP_H

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#include <stdbool.h>
#include <stdlib.h>
typedef struct Heap{
    struct HeapNode* root;
}Heap;

typedef struct HeapNode{
    char* data;
    int dataSum;
    struct HeapNode* left;
    struct HeapNode* right;
    struct HeapNode* parent;
}HeapNode;

Heap* getSubHeapTree(Heap* primalTree, HeapNode* newRoot);

Heap* createHeap();

HeapNode* getLeft(HeapNode* A);

HeapNode* getRight(HeapNode* A);

HeapNode* getParent(HeapNode* A);

void exchange(HeapNode** A, HeapNode** i);

void maxHeapify(HeapNode* A);

HeapNode* createHeapNode(char* data, int dataSum);

int maxDepth(HeapNode* root);

HeapNode* insertHeap(Heap* root, HeapNode* newNode);

void printGivenLevel(HeapNode* root, int level, int *k);

void printLevelOrder(HeapNode* root, int k);

HeapNode* ifNodeExists(HeapNode* node, char* key);

void freeHeapNode(Heap* root, HeapNode* node);

void freeHeapTree(Heap* heapTree);

void freeNode(HeapNode* node);

#endif //DISEASEMONITOR_BINARYMAXHEAP_H
//topk_Diseases 5 Italy 1-1-2000 16-1-2006