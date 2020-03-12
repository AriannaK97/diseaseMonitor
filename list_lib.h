//
// Created by AriannaK97 on 10/3/20.
//

#ifndef DISEASEMONITOR_LIST_LIB_H
#define DISEASEMONITOR_LIST_LIB_H

#include <stdlib.h>
#include <stdio.h>


typedef struct node{
    void* item ;
    struct node* next;
}Node;

typedef struct list{
    Node* head;
    Node* tail;
}List;


Node* popNode(List* linkedList);
Node* nodeInit(void* item);
List* linkedListInit(Node* node);
void push(Node* listNode, List* linkedList);
void listMemoryDeallock(List* linkedList);
void printList(List* patientList);

#endif //DISEASEMONITOR_LIST_LIB_H