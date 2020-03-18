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
    if(bucket->numOfEntries < entries){
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
    bucket->bucketSize = bucketSize;
    unsigned long freeSpace = bucket->bucketSize - sizeof(Bucket*) - sizeof(int) - sizeof(size_t);
    unsigned long entries = freeSpace/DATA_SPACE;
    bucket->entry = malloc(entries * sizeof(BucketEntry));
    for(int i = 0; i < entries; i++){
        bucket->entry[i].data = malloc(DATA_SPACE* sizeof(char));
        bucket->entry[i].tree = (struct rbTree *) createRbTree();
    }

    memcpy(bucket->entry[0].data, data, DATA_SPACE);
    bucket->entry[0].key = key;
    //create the rbtree for the new entry
    rbNode* treeNode = createRbTreeNode(listNode);
    rbInsert((rbTree*)bucket->entry[0].tree, treeNode);
    bucket->numOfEntries++;

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
    int bCounter = 0;
    while (bucket != NULL){

        for(int i = 0; i < bucket->numOfEntries; i++){
            if(strcmp(data, bucket->entry[i].data)==0){
                rbNode* treeNode = createRbTreeNode(listNode);
                rbInsert((rbTree*)bucket->entry[i].tree, treeNode);

/*                if(strcmp(bucket->entry[i].data, "France")==0){
                    rbTree* tree =  bucket->entry[i].tree;
                    printf("I am bucket %d\n", bCounter);
                    printRbTree(tree->root, 0);
                    printf("/////////////////////////////////////////////////////////////////////////\n");
                }*/

                return;
            }
        }

        //we have the final bucket of the list and have not found any matches previously
        if(bucket->next == NULL){

            if(bucketHasSpace(bucket)){

                memcpy(bucket->entry[bucket->numOfEntries].data, data, DATA_SPACE);
                bucket->entry[bucket->numOfEntries].key = key;
                //create the rbtree for the new entry
                rbNode* treeNode = createRbTreeNode(listNode);
                rbInsert((rbTree*)bucket->entry[bucket->numOfEntries].tree, treeNode);

                bucket->numOfEntries++;
                hTable->e_num ++;

                return;
            }else{

                int freeSpace = bucket->bucketSize - sizeof(Bucket*) - sizeof(int) - sizeof(size_t);
                int entries = freeSpace/DATA_SPACE;

                bucket = getBucket(bucketSize, bucket);

                bucket->entry = malloc(entries * sizeof(BucketEntry));
                for(int i = 0; i < entries; i++){
                    bucket->entry[i].data = malloc(DATA_SPACE* sizeof(char));
                    bucket->entry[i].tree = createRbTree();
                }

                memcpy(bucket->entry[0].data, data, DATA_SPACE);
                bucket->entry[0].key = key;
                //create the rbtree for the new entry
                rbNode* treeNode = createRbTreeNode(listNode);
                rbInsert((rbTree*)bucket->entry[0].tree, treeNode);

                bucket->numOfEntries++;
                hTable->e_num ++;

                return;
            }
        }

        bucket = bucket->next;
        bCounter++;
    }
}

/**
 * Retrieve data from the hashtable
 */
void* hashGet(HashTable* hTable, unsigned long key){
    unsigned int h = hash(key) % hTable->capacity;
    Bucket* bucket = hTable->table[h];
    while(bucket != NULL){
        for (int i = 0; i < bucket->numOfEntries; i++)
            if(bucket->entry[i].key == key) //cover collision cases
                return bucket->entry[i].data;
        bucket = bucket->next;
    }
    return NULL;
}

/**
 * Remove data from the hashtable. Return the data removed from the table
 * so that we can free memory if needed.
 **/
void* hashRemove(HashTable* hTable, unsigned long key){
    unsigned int h = hash(key) % hTable->capacity;
    Bucket* bucket = hTable->table[h];
    Bucket* prev = NULL;
    while(bucket != NULL){
        for (int i = 0; bucket->numOfEntries; i++){
            if(bucket->entry[i].key == key){
                void* ret = bucket->entry->data;
                if(prev != NULL)
                    prev->next = bucket->next;
                else
                    hTable->table[h] = bucket->next;
                free(bucket);
                bucket = NULL;
                hTable->e_num --;
                return ret;
            }
            prev = bucket;
        }
        bucket = bucket->next;
    }
    return NULL;
}

/**
 * List keys. k should have length equals or greater than the number of keys
 */
/*void hashListKeys(HashTable* hTable, unsigned long* k, size_t len){
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
}*/

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


void iterateBucketData(Bucket* bucket, int operationCall){
    BucketEntry *iterator = bucket->entry;

    Date searchDate;
    searchDate.day = 18;
    searchDate.month = 05;
    searchDate.year = 2012;

    if(operationCall == SEARCH){
        for(int i = 0; i < bucket->numOfEntries; i++){
            if(iterator[i].tree != NULL){
                searchRbNode((rbTree*)iterator[i].tree, &searchDate);
            }
            printf("%s\n",iterator[i].data);
            }
    }else if(operationCall == REMOVE){
        for(int i = 0; i < bucket->numOfEntries; i++){
            if(bucket != NULL){
                if(iterator[i].tree != NULL){
                    freeRbTree((rbTree*)iterator[i].tree);
                }
            }
        }
    }
}


/**
 * Iterate through table's elements.
 */
Bucket* hashIterate(HashElement* iterator, int operationCall){
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
        if(operationCall == SEARCH){
            iterateBucketData(bucket, SEARCH);
            iterator->elem = bucket->next;
        }else if(operationCall == REMOVE){
            iterateBucketData(bucket, REMOVE);
            iterator->elem = bucket->next;
        }
    }
    return bucket;
}


/* Iterate through keys. */
unsigned long hashIterateKeys(HashElement* iterator, int operationCall){
    Bucket* e = hashIterate(iterator, operationCall);
    return (e == NULL ? 0 : e->entry->key);
}

/**
 * Iterate through values.
 */
void* hashIterateValues(HashElement* iterator, int operationCall){
    Bucket* e = hashIterate(iterator, operationCall);
    return (e == NULL ? NULL : e->entry->data);
}

/**
 * Removes all elements stored in the hashtable.
 * if free_data, all stored datas are also freed.
 */
void hashClear(HashTable* hTable, int operationCall){
    HashElement it = hashITERATOR(hTable);
    unsigned long k = hashIterateKeys(&it, REMOVE);
    while(k != 0){
        operationCall ? free(hashRemove(hTable, k)) : hashRemove(hTable, k);
        k = hashIterateKeys(&it, REMOVE);
    }
}

/**	Destroy the hash table, and free memory.
  * Data still stored are freed
  */
void hashDestroy(HashTable* hTable){
    hashClear(hTable, REMOVE); // Delete and free all.
    free(hTable->table);
    free(hTable);
}


void printHashTable(HashTable* hTable){
    HashElement *iterator = malloc(sizeof(HashElement));
    char *data = malloc(DATA_SPACE*sizeof(char));
    iterator->ht = hTable;
    iterator->index = 0;
    iterator->elem = NULL;

    while((data = hashIterateValues(iterator, SEARCH)) != NULL);
    free(iterator);
    free(data);
}