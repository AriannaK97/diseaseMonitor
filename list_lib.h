//
// Created by AriannaK97 on 10/3/20.
//

#ifndef DISEASEMONITOR_LIST_LIB_H
#define DISEASEMONITOR_LIST_LIB_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "structs.h"
#include "data_io.h"

/*typedef struct Node{
    void* item;
    struct Node* next;
}Node;

typedef struct List{
    struct Node* head;
    struct Node* tail;
}List;*/

Node* popNode(List* linkedList);
Node* nodeInit(void* item);
List* linkedListInit(Node* node);
void push(Node* listNode, List* linkedList);
void listMemoryDeallock(List* linkedList);
void printList(List* patientList);
bool searchListForDuplicates(List* linkedList, int key);
void listClear(List* linkedList);

#endif //DISEASEMONITOR_LIST_LIB_H
