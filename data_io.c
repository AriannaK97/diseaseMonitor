//
// Created by AriannaK97 on 9/3/20.
//
#include "data_io.h"

FILE* openFile(char *inputFile){
    FILE *patientRecordsFile;

    patientRecordsFile = fopen(inputFile, "r");

    if (patientRecordsFile == NULL) {

        fprintf(stderr, "File %s, could not be opened. Are you sure this file exists?\n", inputFile);
        exit(1);
    } else {

        fprintf(stdout, "Files %s was succesfully opened.\n", inputFile);
    }
    return patientRecordsFile;
}

/***
 * Get the max value of either the longest line in file or the buffersize->max lines in file
 * returnVal accepts a value from enum {LINE_LENGTH, BUFFER_SIZE}, returning the adequate
 * value respectively.
 * */
int getMaxFromFile(FILE* patientRecordsFile, int returnVal){
    int c = 0;
    int BUFFERSIZE = 0;
    int maxStringLength = 0;
    int currentStringLength = 0;

    while(c != EOF){
        c = fgetc(patientRecordsFile);
        currentStringLength += 1;
        if(c == '\n'){
            BUFFERSIZE++;
            if(currentStringLength > maxStringLength){  //looking for the longest line in the textfile
                maxStringLength = currentStringLength;
                currentStringLength = 0;
            }
        }
    }
    rewind(patientRecordsFile);     /*return file pointer back to the top of the file*/

    if (c == EOF){
        BUFFERSIZE += 1; //the last line read is not counted
    }

    if (returnVal == LINE_LENGTH) {
        return maxStringLength;
    }else if (returnVal == BUFFER_SIZE) {
        return BUFFERSIZE;
    }
}


PatientCase* getPatient(char* buffer){
    const char* delim = " ";
    const char* dateDelim = " -";
    int tokenCase = 0;
    char* token = NULL;
    PatientCase *newPatient = malloc(sizeof(struct PatientCase));

    token = strtok(buffer, delim);
    while(tokenCase != 12 && token != NULL){
        if (tokenCase == 0){
            newPatient->caseNum = atoi(token);
            token = strtok(NULL, delim);
        } else if (tokenCase == 1){
            strcpy(newPatient->name, token);
            token = strtok(NULL, delim);
        }else if (tokenCase == 2){
            strcpy(newPatient->surname, token);
            token = strtok(NULL, delim);
        }else if (tokenCase == 3){
            strcpy(newPatient->virus, token);
            token = strtok(NULL, delim);
        }else if (tokenCase == 4){
            strcpy(newPatient->country, token);
            token = strtok(NULL, dateDelim);
        }else if (tokenCase == 5){
            newPatient->importDate.day = atoi(token);
            token = strtok(NULL, dateDelim);
        }else if (tokenCase == 6){
            newPatient->importDate.month = atoi(token);
            token = strtok(NULL, dateDelim);
        }else if (tokenCase == 7){
            newPatient->importDate.year = atoi(token);
            token = strtok(NULL, dateDelim);
            /*if there is no next token for the export date we infer that there is no such
             * and assign the arbitrary value -1*/
            if(token == NULL){
                newPatient->exportDate.day = 0;
                newPatient->exportDate.month = 0;
                newPatient->exportDate.year = 0;
                tokenCase = 9;
            }
        }else if (tokenCase == 8){
            newPatient->exportDate.day = atoi(token);
            token = strtok(NULL, dateDelim);
        }else if (tokenCase == 9){
            newPatient->exportDate.month = atoi(token);
            token = strtok(NULL, dateDelim);
        }else if (tokenCase == 10){
            newPatient->exportDate.year = atoi(token);
            token = strtok(NULL, dateDelim);
        }
        tokenCase++;
    }
    tokenCase = 0;
//    fprintf(stdout,"case number: %d | name: %s | surname: %s | virus: %s | country: %s | importDate: %d-%d-%d | "
//                   "exportDate: %d-%d-%d\n", newPatient->caseNum, newPatient->name, newPatient->surname, newPatient->virus,
//                   newPatient->country, newPatient->importDate.day, newPatient->importDate.month, newPatient->importDate.year
//                   ,newPatient->exportDate.day, newPatient->exportDate.month, newPatient->exportDate.year);
    return newPatient;
}


List* read_input_file(FILE* patientRecordsFile, size_t maxStrLength, HashTable** diseaseHashTable,
                        HashTable** countryHashTable, size_t bucketSize){
    char *buffer = malloc(sizeof(char)*maxStrLength);
    PatientCase *newPatient;
    Node* newNode;
    List* patientList = NULL;

    while(getline(&buffer, &maxStrLength, patientRecordsFile) >= 0){
        newPatient = getPatient(buffer);
        newNode = nodeInit(newPatient);
        if(patientList == NULL){
            patientList = linkedListInit(newNode);
        }else{
            push(newNode, patientList);
        }
        hashPut(*diseaseHashTable, strlen(newPatient->virus), newPatient->virus, bucketSize);
        hashPut(*countryHashTable, strlen(newPatient->country), newPatient->country, bucketSize);

    }
    printHashTable(*diseaseHashTable);
    printHashTable(*countryHashTable);
    //printList(patientList);

    return patientList;
}