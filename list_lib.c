//
// Created by AriannaK97 on 10/3/20.
//
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

bool compareListItemPatient(PatientCase* patient, int key){
    if (patient->caseNum == key){
        return true;
    }
    return false;
}

bool searchListForDuplicates(List* linkedList, int key){
    Node* node = linkedList->head;
    while (node != NULL){
        if (compareListItemPatient(node->item, key)){
            fprintf(stderr, "System stopped due to input error.\nThe recordID already exists in the system.\n Exit...\n");
            exit(1);
        }
        node = node->next;
    }
    return false;

}

void printList(List* patientList){
    PatientCase *newPatient;
    Node* newNode;
    while (patientList->head != NULL){
        newNode = popNode(patientList);
        newPatient = newNode->item;
        fprintf(stdout,"case number: %d | name: %s | surname: %s | virus: %s | country: %s | importDate: %d-%d-%d | "
                       "exportDate: %d-%d-%d\n", newPatient->caseNum, newPatient->name, newPatient->surname, newPatient->virus,
                newPatient->country, newPatient->entryDate->day, newPatient->entryDate->month, newPatient->entryDate->year
                ,newPatient->exitDate->day, newPatient->exitDate->month, newPatient->exitDate->year);
    }
}

/**
 * List memory deallocation
 * */
void listClear(List* linkedList){
    Node* node = linkedList->head;
    while (node != NULL){
        linkedList->head = linkedList->head->next;
        free(node);
        node = linkedList->head;
    }
    free(linkedList);
}