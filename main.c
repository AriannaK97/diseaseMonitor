#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_io.h"

#define BUCKET_SIZE 68 //minimum bucket size for just one entry

int main(int argc, char** argv) {

    size_t bucketSize = BUCKET_SIZE;
    int diseaseHashtableNumOfEntries = 10;
    int countryHashTableNumOfEntries = 4;
    char *inputFile = NULL;
    int numOfArgs = 0;

/*****************************************************************************
 *                       Handling command line arguments                     *
 *****************************************************************************/
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-p") == 0) {
            inputFile = argv[i + 1];
            numOfArgs += 2;
        } else if (strcmp(argv[i], "-h1") == 0) {
            diseaseHashtableNumOfEntries = atoi(argv[i + 1]);
            numOfArgs += 2;
        } else if (strcmp(argv[i], "-h2") == 0) {
            countryHashTableNumOfEntries = atoi(argv[i + 1]);
            numOfArgs += 2;
        } else if (strcmp(argv[i], "-b") == 0) {
            if(atoi(argv[i + 1]) >= BUCKET_SIZE){
                bucketSize = atoi(argv[i + 1]);
                numOfArgs += 2;
            }else{
                fprintf(stderr, "The BUCKET_SIZE you have provided is invalid! Provide a BUCKET_SIZE >= %d\n", BUCKET_SIZE);
                scanf("%ld" ,&bucketSize);
            }
        } else {
            fprintf(stderr, "Unknown option %s\n", argv[i]);
            exit(1);
        }
    }
    if (inputFile == NULL) {
        fprintf(stdout, "Default file patientRecordsFile loaded...\n");
    }

/*****************************************************************************
 *                       Handling input files                                *
 *****************************************************************************/
    FILE *patientRecordsFile;
    patientRecordsFile = openFile(inputFile);
    size_t lineLenght;
    List* patientList;
    HashTable* diseaseHashTable;
    HashTable* countryHashTable;

    lineLenght = getMaxFromFile(patientRecordsFile, LINE_LENGTH);

    diseaseHashTable = hashCreate(diseaseHashtableNumOfEntries);
    countryHashTable = hashCreate(countryHashTableNumOfEntries);

    patientList = read_input_file(patientRecordsFile, lineLenght, &diseaseHashTable, &countryHashTable, bucketSize);

    return 0;
}