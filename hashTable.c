//
// Created by AriannaK97 on 12/3/20.
//

#include "hashTable.h"

#include <stdlib.h>
#include <string.h>
#include "hashTable.h"


unsigned long hash(unsigned long x){
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

/**
 * Create a hashtable with capacity 'capacity'
 * and return a pointer to it
 */
HashTable* hashCreate(unsigned int capacity){
    HashTable* hTable = malloc(sizeof(HashTable));
    if(!hTable){
        return NULL;
    }
    if((hTable->table = malloc(capacity*sizeof(HashItem*))) == NULL){
        free(hTable->table);
        return NULL;
    }
    hTable->capacity = capacity;
    hTable->e_num = 0;
    unsigned int i;
    for(i = 0; i < capacity; i++){
        hTable->table[i] = NULL;
    }
    return hTable;
}

/**
 * Store data in the hashtable. If data with the same key are already stored,
 * they are overwritten, and return by the function. Else it return NULL.
 * Return hashERROR if there are memory alloc error
 */
void* hashPut(HashTable* hTable, unsigned long key, void* data){
    if(data == NULL){
        return NULL;
    }
    unsigned int h = hash(key) % hTable->capacity;
    HashItem* e = hTable->table[h];

    while(e != NULL){
        if(e->key == key){
            void* ret = e->data;
            e->data = data;
            return ret;
        }
        e = e->next;
    }

    // Getting here means the key doesn't already exist

    if((e = malloc(sizeof(HashItem)+ sizeof(unsigned long)+1)) == NULL){
        //return hashERROR;
        return NULL;
    }
    e->key = key;
    e->data = data;

    // Add the element at the beginning of the linked list
    e->next = hTable->table[h];
    hTable->table[h] = e;
    hTable->e_num ++;

    return NULL;
}

/**
 * Retrieve data from the hashtable
 */
void* hashGet(HashTable* hTable, unsigned long key){
    unsigned int h = hash(key) % hTable->capacity;
    HashItem* e = hTable->table[h];
    while(e != NULL){
        if(e->key == key) //cover collision cases
            return e->data;
        e = e->next;
    }
    return NULL;
}

/**
 * Remove data from the hashtable. Return the data removed from the table
 * so that we can free memory if needed.
 **/
void* hashRemove(HashTable* hTable, unsigned long key){
    unsigned int h = hash(key) % hTable->capacity;
    HashItem* e = hTable->table[h];
    HashItem* prev = NULL;
    while(e != NULL){
        if(e->key == key){
            void* ret = e->data;
            if(prev != NULL)
                prev->next = e->next;
            else
                hTable->table[h] = e->next;
            free(e);
            e = NULL;
            hTable->e_num --;
            return ret;
        }
        prev = e;
        e = e->next;
    }
    return NULL;
}

/**
 * List keys. k should have length equals or greater than the number of keys
 */
void hashListKeys(HashTable* hTable, unsigned long* k, size_t len){
    if(len < hTable->e_num){
        return;
    }
    int ki = 0; //Index to the current string in **k
    unsigned int i = hTable->capacity;
    while(--i >= 0)
    {
        HashItem* e = hTable->table[i];
        while(e)
        {
            k[ki++] = e->key;
            e = e->next;
        }
    }
}

/**	List values. v should have length equals or greater
  *than the number of stored elements
  */
void hashListValues(HashTable* hTable, void** v, size_t len){
    if(len < hTable->e_num)
        return;
    int vi = 0; //Index to the current string in **v
    unsigned int i = hTable->capacity;
    while(--i >= 0){
        HashItem* e = hTable->table[i];
        while(e){
            v[vi++] = e->data;
            e = e->next;
        }
    }
}

/**
 * Iterate through table's elements.
 */
HashItem* hashIterate(HashElement* iterator){
    while(iterator->elem == NULL){
        if(iterator->index < iterator->ht->capacity - 1){
            iterator->index++;
            iterator->elem = iterator->ht->table[iterator->index];
        }
        else{
            return NULL;
        }
    }
    HashItem* e = iterator->elem;
    if(e){
        iterator->elem = e->next;
    }
    return e;
}

/* Iterate through keys. */
unsigned long hashIterateKeys(HashElement* iterator){
    HashItem* e = hashIterate(iterator);
    if (e == NULL){
        return NULL;
    }else{
        return e->key;
    }
}

/**
 * Iterate through values.
 */
void* hashIterateValues(HashElement* iterator){
    HashItem* e = hashIterate(iterator);
    return (e == NULL ? NULL : e->data);
}

/**
 * Removes all elements stored in the hashtable.
 * if free_data, all stored datas are also freed.
 */
void hashClear(HashTable* hTable, int free_data){
    HashElement it = hashITERATOR(hTable);
    unsigned long k = hashIterateKeys(&it);
    while(k != NULL){
        free_data ? free(hashRemove(hTable, k)) : hashRemove(hTable, k);
        k = hashIterateKeys(&it);
    }
}

/**	Destroy the hash table, and free memory.
  * Data still stored are freed
  */
void hashDestroy(HashTable* hTable){
    hashClear(hTable, 1); // Delete and free all.
    free(hTable->table);
    free(hTable);
}