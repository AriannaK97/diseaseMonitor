//
// Created by linuxuser on 15/3/20.
//

#ifndef DISEASEMONITOR_STRUCTS_H
#define DISEASEMONITOR_STRUCTS_H

typedef struct BucketEntry BucketEntry;
typedef struct Bucket Bucket;
typedef struct HashTable HashTable;
typedef struct HashElement HashElement;

typedef struct Date{
    int day;
    int month;
    int year;
}Date;

typedef struct PatientCase{
    int caseNum;
    char* name;
    char* surname;
    char* virus;
    char* country;
    struct Date* entryDate;
    struct Date* exitDate;
}PatientCase;

typedef struct Node{
    void* item;
    struct Node* next;
}Node;

typedef struct List{
    struct Node* head;
    struct Node* tail;
}List;

#endif //DISEASEMONITOR_STRUCTS_H