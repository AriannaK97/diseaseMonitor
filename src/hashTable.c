//
// Created by AriannaK97 on 12/3/20.
//

#include "../header/hashTable.h"


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
    if((hTable->table = malloc(capacity*sizeof(Bucket*))) == NULL){
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

bool bucketHasSpace(Bucket *bucket){
    //the new data can fit in the existing bucket
    int freeSpace = bucket->bucketSize - sizeof(Bucket*) - sizeof(int) - sizeof(size_t);
    int entries = freeSpace/DATA_SPACE;
    if(entries > bucket->numOfEntries){
        return true;
    }else{
        return false;
    }
}

/**
 * When the previous bucket is full and only.
 * */
Bucket* getBucket(size_t bucketSize, Bucket *prevBucket){
    Bucket *e;
    if((e = malloc(sizeof(Bucket))) == NULL){
        return NULL;
    }
    e->bucketSize = prevBucket->bucketSize;
    prevBucket->next = e;
    e->next = NULL;

    return e;
}


/**
 * Store data in the hashtable. If data with the same key are already stored,
 * they are overwritten, and return by the function. Else it return NULL.
 * Return hashERROR if there are memory alloc error
 */
void* hashPut(HashTable* hTable, unsigned long key, void* data, size_t bucketSize, Node* listNode){
    if(data == NULL){
        return NULL;
    }
    unsigned int h = hash(key) % hTable->capacity;
    Bucket* bucket = hTable->table[h];

    if(bucket != NULL){
        putInBucketData(bucket, bucketSize, data, hTable, key, listNode);
        return NULL;
    }

    // Getting here means the key doesn't already exist

    if((bucket = malloc(sizeof(Bucket))) == NULL){
        //return hashERROR;
        return NULL;
    }
    bucket->key = key;
    bucket->bucketSize = bucketSize;
    int freeSpace = bucket->bucketSize - sizeof(Bucket*) - sizeof(int) - sizeof(size_t);
    int entries = freeSpace/DATA_SPACE;
    bucket->entry = malloc(entries * sizeof(BucketEntry));
    for(int i = 0; i < entries; i++){
        bucket->entry[i].data = malloc(DATA_SPACE* sizeof(char));
    }
    memcpy(bucket->entry[0].data, data, DATA_SPACE);
    bucket->numOfEntries++;

    //create the rbtree for the new entry
    bucket->entry->tree = createRbTree();
    rbNode* treeNode = createRbTreeNode(listNode);
    rbInsert(bucket->entry->tree, treeNode);

    // Add the element at the beginning of the linked list
    bucket->next = hTable->table[h];
    hTable->table[h] = bucket;
    hTable->e_num ++;

    return NULL;
}

/**
 * either the virus or the country occupy 32bytes
 * */
void putInBucketData(Bucket* bucket, size_t bucketSize, char* data, HashTable* hTable, unsigned long key, Node* listNode){

    while (bucket != NULL){
        if(bucket->key == key){
            for(int i = 0; i < bucket->numOfEntries; i++){
                if (strcmp(data, bucket->entry[i].data)==0){
                    rbNode* treeNode = createRbTreeNode(listNode);
                    rbInsert(bucket->entry->tree, treeNode);
                    return;
                }
            }

            //we have the final bucket of the list and have not found any matches previously
            if(bucket->next == NULL){
                if(bucketHasSpace(bucket)){
                    memcpy(bucket->entry[bucket->numOfEntries].data, data, DATA_SPACE);
                    bucket->numOfEntries++;
                    hTable->e_num ++;

                    //create the rbtree for the new entry
                    bucket->entry->tree = createRbTree();
                    rbNode* treeNode = createRbTreeNode(listNode);
                    rbInsert(bucket->entry->tree, treeNode);

                    return;
                } else{
                    bucket = getBucket(bucketSize, bucket);
                    bucket->key = key;
                    int freeSpace = bucket->bucketSize - sizeof(Bucket*) - sizeof(int) - sizeof(size_t);
                    int entries = freeSpace/DATA_SPACE;
                    bucket->entry = malloc(entries * sizeof(BucketEntry));
                    for(int i = 0; i < entries; i++){
                        bucket->entry[i].data = malloc(DATA_SPACE* sizeof(char));
                    }
                    memcpy(bucket->entry[0].data, data, DATA_SPACE);
                    bucket->numOfEntries++;
                    hTable->e_num ++;

                    //create the rbtree for the new entry
                    bucket->entry->tree = createRbTree();
                    rbNode* treeNode = createRbTreeNode(listNode);
                    rbInsert(bucket->entry->tree, treeNode);

                    return;
                }
            }
        }
        bucket = bucket->next;
    }
}

/**
 * Retrieve data from the hashtable
 */
void* hashGet(HashTable* hTable, unsigned long key){
    unsigned int h = hash(key) % hTable->capacity;
    Bucket* e = hTable->table[h];
    while(e != NULL){
        if(e->key == key) //cover collision cases
            return e->entry->data;
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
    Bucket* e = hTable->table[h];
    Bucket* prev = NULL;
    while(e != NULL){
        if(e->key == key){
            void* ret = e->entry->data;
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
        Bucket* e = hTable->table[i];
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
        Bucket* e = hTable->table[i];
        while(e){
            v[vi++] = e->entry->data;
            e = e->next;
        }
    }
}


void iterateBucketData(Bucket* bucket){
    BucketEntry *iterator = bucket->entry;

    for(int i = 0; i < bucket->numOfEntries; i++){
        printf("%s\n",iterator[i].data);
    }
}


/**
 * Iterate through table's elements.
 */
Bucket* hashIterate(HashElement* iterator){
    while(iterator->elem == NULL){
        if(iterator->index < iterator->ht->capacity - 1){
            iterator->index++;
            iterator->elem = iterator->ht->table[iterator->index];
        }
        else{
            return NULL;
        }
    }
    Bucket* bucket = iterator->elem;
    if(bucket){
        iterateBucketData(bucket);
        iterator->elem = bucket->next;
    }
    return bucket;
}

/* Iterate through keys. */
unsigned long hashIterateKeys(HashElement* iterator){
    Bucket* e = hashIterate(iterator);
    return (e == NULL ? NULL : e->key);
}

/**
 * Iterate through values.
 */
void* hashIterateValues(HashElement* iterator){
    Bucket* e = hashIterate(iterator);
    return (e == NULL ? NULL : e->entry->data);
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


void printHashTable(HashTable* hTable){
    HashElement *iterator = malloc(sizeof(HashElement));
    iterator->ht = hTable;
    iterator->index = 0;
    iterator->elem = NULL;
    char *data = malloc(DATA_SPACE*sizeof(char));

    while((data = hashIterateValues(iterator)) != NULL);
}