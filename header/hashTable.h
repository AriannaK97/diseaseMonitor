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

#define DATA_SPACE 32
#define REMOVE 1
#define SEARCH 2

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
    int BucketEntry;
}HashElement;


// Inititalize hashtable iterator on hashtable 'ht'
#define hashITERATOR(ht) {ht, 0, ht->table[0]}

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

void printHashTable(HashTable* hTable);

void putInBucketData(Bucket* bucket, size_t bucketSize, char* data, HashTable* hTable, unsigned long key, Node* listNode);

void iterateBucketData(Bucket* bucket, int operationCall);

#endif //DISEASEMONITOR_HASHTABLE_H
