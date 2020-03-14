//
// Created by AriannaK97 on 9/3/20.
//

#ifndef DISEASEMONITOR_DATA_IO_H
#define DISEASEMONITOR_DATA_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_lib.h"
#include "hashTable.h"

#define DATA_SPACE 32

struct Date{
    int day;
    int month;
    int year;
};

typedef struct PatientCase{
    int caseNum;
//    char name[32];
//    char surname[32];
//    char virus[32];
//    char country[32];
    char* name;
    char* surname;
    char* virus;
    char* country;
    struct Date importDate;
    struct Date exportDate;
}PatientCase;

enum {
    LINE_LENGTH,
    BUFFER_SIZE,
};

FILE* openFile(char *inputFile);
int getMaxFromFile(FILE* patientRecordsFile, int returnVal);

List* read_input_file(FILE* patientRecordsFile, size_t maxStrLength, HashTable** diseaseHashTable,
                      HashTable** countryHashTable, size_t bucketSize);

#endif //DISEASEMONITOR_DATA_IO_H
