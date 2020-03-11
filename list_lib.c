//
// Created by AriannaK97 on 10/3/20.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list_lib.h"

Node* nodeInit(void* item){

    Node* newNode =  malloc(sizeof(Node));

    newNode->item = item;
    newNode->next = NULL;

    return newNode;
}

List* linkedListInit(Node* node){
    List* newList = malloc(sizeof(List));
    newList->head = node;
    newList->tail = node;
    return newList;
}

void push(Node* listNode, List* linkedList){

    if(linkedList->head == NULL){
        linkedList->head = listNode;
        linkedList->tail = listNode;
    }else{
        linkedList->tail->next = listNode;
        linkedList->tail = listNode;
    }
}


void listMemoryDeallock(List* linkedList){
    Node* listNode = linkedList->head;
    while(listNode != NULL){
        linkedList->head = linkedList->head->next;
        free(listNode);
        listNode = linkedList->head;
    }
    free(linkedList);
}


Node* popNode(List* linkedList){
    Node* node = linkedList->head;
    linkedList->head = linkedList->head->next;
    if(linkedList->head == NULL){
        linkedList->tail = NULL;
    }
    return node;
}