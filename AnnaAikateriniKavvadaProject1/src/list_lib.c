//
// Created by AriannaK97 on 10/3/20.
//
#include "../header/list_lib.h"


Node* nodeInit(void* item){

    Node* newNode =  (struct Node*)malloc(sizeof(Node));

    newNode->item = item;
    newNode->next = NULL;

    return newNode;
}

List* linkedListInit(Node* node){
    List* newList = (struct List*)malloc(sizeof(List));
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

/**
 * List memory deallocation
 * */
void listMemoryDeallock(List* linkedList){
    Node* listNode = linkedList->head;
    while(listNode != NULL){
        linkedList->head = linkedList->head->next;
        nodeItemDeallock(listNode->item);
        free(listNode);
        listNode = linkedList->head;
    }
    free(linkedList);
}

void nodeItemDeallock(PatientCase* item){
    free(item->entryDate);
    free(item->exitDate);
    free(item->name);
    free(item->surname);
    free(item->country);
    free(item->virus);
    free(item);
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

bool searchNodeForRecordID_ExitDateUpdate(List* linkedList, int key, Date* exitDate){
    Node* node = linkedList->head;
    while (node != NULL){
        if (compareListItemPatient(node->item, key)){
            PatientCase* exitPatient = (PatientCase*)node->item;
            if(dateInputValidation(exitPatient->entryDate, exitDate)){
                exitPatient->exitDate = exitDate;
                return true;
            }else{
                fprintf(stdout,"Exit date could not be updated for patient:\n case number: %d | name: %s | "
                               "surname: %s | virus: %s | country: %s | entryDate: %d-%d-%d\n\nInvalid exit date: "
                               "%d-%d-%d", exitPatient->caseNum, exitPatient->name, exitPatient->surname,
                               exitPatient->virus, exitPatient->country, exitPatient->entryDate->day,
                               exitPatient->entryDate->month, exitPatient->entryDate->year, exitPatient->exitDate->day,
                               exitPatient->exitDate->month, exitPatient->exitDate->year);
                return false;
            }
        }
        node = node->next;
    }
    fprintf(stderr, "Could not find the patient with record id %d in the system\n Could not update", key);
    return false;
}

/**
 * Search for duplicates in list
 * Used for new entry validation
 * */
bool searchListForRecordID(List* linkedList, int key){
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

/**
 * Print list
 * */
void printList(List* patientList){
    PatientCase* newPatient;
    Node* newNode = patientList->head;
    while (newNode != NULL && newNode->item != NULL){
        newPatient = (PatientCase*)newNode->item;
        fprintf(stdout,"case number: %d | name: %s | surname: %s | virus: %s | country: %s | importDate: %d-%d-%d | "
                       "exportDate: %d-%d-%d\n", newPatient->caseNum, newPatient->name, newPatient->surname, newPatient->virus,
                newPatient->country, newPatient->entryDate->day, newPatient->entryDate->month, newPatient->entryDate->year
                ,newPatient->exitDate->day, newPatient->exitDate->month, newPatient->exitDate->year);

        newNode = newNode->next;
    }
}

/**
 * Print a certain given node of type Node
 * */
void printListNode(Node* node){
    if(node == NULL){
        return;
    }
    PatientCase* patient = node->item;
    fprintf(stdout,"case number: %d | name: %s | surname: %s | virus: %s | country: %s | importDate: %d-%d-%d | "
                   "exportDate: %d-%d-%d\n", patient->caseNum, patient->name, patient->surname, patient->virus,
                   patient->country, patient->entryDate->day, patient->entryDate->month, patient->entryDate->year,
                   patient->exitDate->day, patient->exitDate->month, patient->exitDate->year);
}