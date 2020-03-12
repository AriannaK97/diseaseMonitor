//
// Created by AriannaK97 on 12/3/20.
//

#ifndef DISEASEMONITOR_HASHTABLE_H
#define DISEASEMONITOR_HASHTABLE_H
#include <stdlib.h>
//Hashtable element structure
typedef struct HashItem {
    struct HashItem* next; // Next element in case of a collision
    void* data;	// Pointer to the stored element
    unsigned long key; 	// Key of the stored element
} HashItem;



//Hashtable structure
typedef struct HashTable{
    unsigned int capacity;	// Hashtable capacity (in terms of hashed keys)
    unsigned int e_num;	// Number of element currently stored in the hashtable
    HashItem** table;	// The table containing elements
} HashTable;




//Structure used for iterations
typedef struct HashElement{
    HashTable* ht; 	// The hashtable on which we iterate
    unsigned int index;	// Current index in the table
    HashItem* elem; 	// Current element in the list
} HashElement;




// Inititalize hashtable iterator on hashtable 'ht'
#define hashITERATOR(ht) {ht, 0, ht->table[0]}




char err_ptr;
//void* hashERROR = &err_ptr; // Data pointing to hashERROR are returned in case of error




unsigned long hash(unsigned long x);

HashTable* hashCreate(unsigned int);

void* hashPut(HashTable*, unsigned long, void*);

void* hashGet(HashTable*, unsigned long);

void* hashRemove(HashTable*, unsigned long);

void hashListKeys(HashTable* hTable, unsigned long* k, size_t len);

void hashListValues(HashTable*, void**, size_t);

HashItem* hashIterate(HashElement*);

unsigned long hashIterateKeys(HashElement*);

void* hashIterateValues(HashElement*);

void hashClear(HashTable*, int);

void hashDestroy(HashTable*);

#endif //DISEASEMONITOR_HASHTABLE_H
