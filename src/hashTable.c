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

int iterateBucketData_BetweenDates(Bucket* bucket, int operationCall, HashElement* hashIterator){
    BucketEntry *iterator = bucket->entry;
    int counter = 0;

    for(int i = 0; i < bucket->numOfEntries; i++){
        if(iterator[i].tree != NULL){
            if(operationCall == SEARCH){
                searchRbNode((rbTree*)iterator[i].tree, hashIterator->date1);
            }else if (operationCall == COUNT_ALL_BETWEEN_DATES){
                counter += countPatients_BetweenDates((rbTree*)iterator[i].tree, operationCall, hashIterator);
                fprintf(stdout,"The number of patients monitored for %s: %d\n",iterator[i].data, counter);
            }else if(operationCall == COUNT_ALL_BETWEEN_DATES_WITH_VIRUS || operationCall == COUNT_ALL_BETWEEN_DATES_WITH_VIRUS_AND_COUNTRY){
                if(strcmp(iterator[i].data, hashIterator->virus)==0){
                    counter += countPatients_BetweenDates((rbTree*)iterator[i].tree, operationCall, hashIterator);
                    fprintf(stdout,"The number of patients monitored for %s: %d\n",iterator[i].data, counter);
                }
            }else if(operationCall == GET_HEAP_NODES_COUNTRY_DATES){
                if(strcmp(iterator[i].data, hashIterator->country)==0){
                    counter += countPatients_BetweenDates((rbTree*)iterator[i].tree, operationCall, hashIterator);
                }
            }else if (operationCall == GET_HEAP_NODES_VIRUS_DATES){
                if(strcmp(iterator[i].data, hashIterator->virus)==0){
                    counter += countPatients_BetweenDates((rbTree*)iterator[i].tree, operationCall, hashIterator);
                }
            }
        }
    }
    return counter;
}


int iterateBucketData(Bucket* bucket, int operationCall, HashElement* hashIterator){
    BucketEntry *iterator = bucket->entry;

    int counter = 0;

    for (int i = 0; i < bucket->numOfEntries; i++) {
        if (iterator[i].tree != NULL) {
            if(operationCall == COUNT_HOSPITALISED || operationCall == COUNT_ALL){
                counter += countPatients((rbTree *) iterator[i].tree, operationCall,NULL);
                fprintf(stdout, "The number of patients monitored for %s: %d\n", iterator[i].data, counter);
            }else if(operationCall == PRINT) {
                fprintf(stdout, "%s\n", iterator[i].data);
            }else if(operationCall == GET_HEAP_NODES_VIRUS){    //for topk_Countries
                if(strcmp(iterator[i].data, hashIterator->virus)==0){
                    counter += countPatients((rbTree*)iterator[i].tree, operationCall, hashIterator);
                }
            }else if(operationCall == GET_HEAP_NODES_COUNTRY){  //for topk_Diseases
                if(strcmp(iterator[i].data, hashIterator->country)==0){
                    counter += countPatients((rbTree*)iterator[i].tree, operationCall, hashIterator);
                }
            }else if(operationCall == REMOVE){
                if(iterator[i].tree != NULL){
                    freeRbTree((rbTree*)iterator[i].tree);
                }
                if(iterator[i].data != NULL){
                    free(iterator[i].data);
                }
            }
        }
    }
    return counter;
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
        if((iterator->date1 == 0 && iterator->date2 == 0) || operationCall == REMOVE){
            /*iterate bucket for querys without defined date gap and and for delete*/
            iterator->counter += iterateBucketData(bucket, operationCall, iterator);
        } else {
            /*iterate in buckets for queries with defined date gap*/
            iterator->counter += iterateBucketData_BetweenDates(bucket, operationCall, iterator);
        }
        iterator->elem = bucket->next;
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
void hashClear(HashTable* hTable, int free_data){
    HashElement it = hashITERATOR(hTable);
    unsigned long k = hashIterateKeys(&it, REMOVE);
    while(k != 0){
        free_data ? free(hashRemove(hTable, k)) : hashRemove(hTable, k);
        k = hashIterateKeys(&it, REMOVE);
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


void applyOperationOnHashTable(HashTable* hTable, int operationCall){
    HashElement iterator = hashITERATOR(hTable);
    while(hashIterateValues(&iterator, operationCall) != NULL);
}