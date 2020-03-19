//
// Created by AriannaK97 on 12/3/20.
//

#ifndef DISEASEMONITOR_HASHTABLE_H
#define DISEASEMONITOR_HASHTABLE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list_lib.h"
#include "redBlackTree.h"
#include "binaryMaxHeap.h"

#define DATA_SPACE 32
#define REMOVE 1
#define SEARCH 2
#define PRINT 3
#define COUNT_ALL 4
#define COUNT_HOSPITALISED 5
#define COUNT_ALL_BETWEEN_DATES 6
#define COUNT_ALL_BETWEEN_DATES_WITH_VIRUS 7
#define COUNT_ALL_BETWEEN_DATES_WITH_VIRUS_AND_COUNTRY 8
#define TOP_K_DISEASES 9
#define TOP_K_DISEASES_DATE 10
#define TOP_K_COUNTRIES 11
#define TOP_K_COUNTRIES_DATE 12

typedef struct BucketEntry{
    char* data;
    struct rbTree* tree;
    unsigned long key;
}BucketEntry;

//Hashtable element structure
typedef struct Bucket {
    BucketEntry* entry;	// Pointer to the stored element 8
    struct Bucket* next; // Next element in case of a collision 8
    int numOfEntries;//the space the data we have already entered occupy in the bucket 4
    size_t bucketSize; //8
}Bucket;

//Hashtable structure
typedef struct HashTable{
    unsigned int capacity;	// Hashtable capacity (in terms of hashed keys)
    unsigned int e_num;	// Number of element currently stored in the hashtable
    Bucket** table;	// The table containing elements
}HashTable;

//Structure used for iterations
typedef struct HashElement{
    HashTable* ht; 	// The hashtable on which we iterate
    unsigned int index;	// Current index in the table
    Bucket* elem; 	// Current element in the list
    int counter;    //used when we count all the entries in the system
    Date* date1;    //for operations requiring date boundaries
    Date* date2;
    char* country; //for operations requiring country
    char* virus;   //for operations requiring the virus for the search
    int k;         //for the top k queries
    HeapNode* maxHeap; //for the top k queries
}HashElement;


// Inititalize hashtable iterator on hashtable 'ht'
#define hashITERATOR(ht) {ht, 0, ht->table[0], 0, 0, 0, 0, 0, 0, 0}

char err_ptr;
//void* hashERROR = &err_ptr; // Data pointing to hashERROR are returned in case of error

unsigned long hash(unsigned long x);

HashTable* hashCreate(unsigned int);

void* hashPut(HashTable* hTable, unsigned long key, void* data, size_t bucketSize, Node* listNode);

void* hashGet(HashTable*, unsigned long);

void hashListValues(HashTable*, void**, size_t);

Bucket* hashIterate(HashElement*, int operationCall);

unsigned long hashIterateKeys(HashElement* iterator, int operationCall);

void* hashIterateValues(HashElement* iterator, int operationCall);

void hashClear(HashTable*, int);

void hashDestroy(HashTable*);

bool bucketHasSpace(Bucket *bucket);

void applyOperationOnHashTable(HashTable* hTable, int operationCall);

void putInBucketData(Bucket* bucket, size_t bucketSize, char* data, HashTable* hTable, unsigned long key, Node* listNode);

int iterateBucketData(Bucket* bucket, int operationCall, HashElement* hashIterator);

int iterateBucketData_BetweenDates(Bucket* bucket, int operationCall, HashElement* hashIterator);

#endif //DISEASEMONITOR_HASHTABLE_H
