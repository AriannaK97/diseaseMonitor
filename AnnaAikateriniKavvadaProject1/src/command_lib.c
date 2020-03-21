//
// Created by AriannaK97 on 12/3/20.
//
#define  _GNU_SOURCE
#include "../header/command_lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../header/structs.h"
#include "../header/hashTable.h"

/**
 * For every disease, print the number of the diseased people monitored in the system.
 * If [date1, date2] are defined we get the number of diseases monitored during the
 * period defined form the given dates.
 * Cmd Args: [date1, date2]
 * */
void globalDiseaseStats(CmdManager* manager, Date* date1, Date* date2){
    HashElement iterator = hashITERATOR(manager->diseaseHashTable);
    if(date1 != NULL && date2 != NULL) {
        iterator.date1 = date1;
        iterator.date2 = date2;
        while (hashIterateValues(&iterator, COUNT_ALL_BETWEEN_DATES) != NULL);
    }else {
        while (hashIterateValues(&iterator, COUNT_ALL) != NULL);
    }
    fprintf(stdout, "Total number of patients counted: %d\n~$:", iterator.counter);
}

/**
 * If the argument [country] is not define, the application prints for the virusName
 * defined the number of the diseased monitored in the system during the specified
 * period between [date1, date2]. If [country] is defined, the application prints
 * the number of the diseased in this [country] for the specified period.
 * Cmd Args: virusName [country] date1 date2
 * */
void diseaseFrequency(CmdManager* manager, char* virusName, Date* date1, Date* date2, char* country){

    HashElement iterator = hashITERATOR(manager->diseaseHashTable);
    iterator.date1 = date1;
    iterator.date2 = date2;
    iterator.virus = virusName;
    if(country == NULL) {
        while (hashIterateValues(&iterator, COUNT_ALL_BETWEEN_DATES_WITH_VIRUS) != NULL);
        fprintf(stdout, "Total number of patients counted: %d\n~$:", iterator.counter);
    }else {
        iterator.country = country;
        while (hashIterateValues(&iterator, COUNT_ALL_BETWEEN_DATES_WITH_VIRUS_AND_COUNTRY) != NULL);
        fprintf(stdout, "Total number of patients counted: %d\n~$:", iterator.counter);
    }
}


/**
 * For the specified country the application prints the diseases with the top k
 * number of diseased cases during the period [date1, date2], if that is specified.
 * Cmd Args: k country [date1, date2]
 * */
void topk_Diseases(CmdManager* manager, int k, char* country, Date* date1, Date* date2){
    HashElement iterator = hashITERATOR(manager->countryHashTable);
    iterator.country = country;
    iterator.k = k;
    iterator.maxHeap = createHeapNode(NULL, 0);
    if(date1 != NULL && date2 != NULL) {
        iterator.date1 = date1;
        iterator.date2 = date2;
        while (hashIterateValues(&iterator, TOP_K_DISEASES_DATE) != NULL);
    }else {
        while (hashIterateValues(&iterator, TOP_K_DISEASES) != NULL);
    }
    freeHeapTree(iterator.maxHeap);
}

/**
 * For the specified disease print the countries with the top k number of cases
 * Cmd Args: k disease [date1, date2]
 * */
void topk_Countries(CmdManager* manager, int k, char* disease, Date* date1, Date* date2){
    HashElement iterator = hashITERATOR(manager->diseaseHashTable);
    iterator.virus = disease;
    iterator.k = k;
    iterator.maxHeap = createHeapNode(NULL, 0);
    if(date1 != NULL && date2 != NULL) {
        iterator.date1 = date1;
        iterator.date2 = date2;
        while (hashIterateValues(&iterator, TOP_K_COUNTRIES_DATE) != NULL);
    }else {
        while (hashIterateValues(&iterator, TOP_K_COUNTRIES) != NULL);
    }
    freeHeapTree(iterator.maxHeap);
}

/**
 * Insert a new patient record in the system
 * Cmd Args: recordID patientFirstName patientLastName diseaseID entryDate [exitDate]
 * */
void insertPatientRecord(CmdManager* manager, char* args){
    size_t maxLength = 256;

    if(writeEntry(args, manager->patientList, manager->diseaseHashTable,
            manager->countryHashTable, manager->bucketSize))
        fprintf(stdout,"New record successfully added.\n~$:");
    else
        fprintf(stderr,"New record failed to be inserted.\n~$:");
    applyOperationOnHashTable(manager->diseaseHashTable, SEARCH);
}


/**
 * Insert exitDate for the given recordID
 * Cmd Args: recordID exitDate
 * */
void recordPatientExit(CmdManager* manager, char* args){
    int recordId;
    Date exitDate;

    recordId = atoi(strtok(args, " "));
    exitDate.day = atoi(strtok(NULL,"-"));
    exitDate.month = atoi(strtok(NULL, "-"));
    exitDate.year = atoi(strtok(NULL, "-"));

    searchNodeForRecordID_ExitDateUpdate(manager->patientList, recordId, &exitDate);
    fprintf(stdout,"~$:");
}


/**
 * If the argument [disease] is given, the application prints the number of the
 * patients hospitalised with the specified disease. If the [disease] is not specified
 * the application prints for all the diseases in the system the records of all the
 * patients that are still hospitalised.
 * Cmd Args: [disease]
 * */
void numCurrentPatients(CmdManager* manager, char* disease){
    int diseaseExists = false;
    if(disease != NULL){
        unsigned int h = hash(strlen(disease)) % manager->diseaseHashTable->capacity;
        Bucket* bucket = manager->diseaseHashTable->table[h];
        if(bucket != NULL){
            while (bucket != NULL){
                for(int i = 0; i < bucket->numOfEntries; i++){
                    if(strcmp(disease, bucket->entry[i].data)==0){
                        int patientsNum = countPatients(bucket->entry[i].tree, COUNT_HOSPITALISED, NULL);
                        fprintf(stdout, "The number of the currently hospitalised patients for %s is: %d\n~$:", disease, patientsNum);
                        diseaseExists = 1;
                        break;
                    }
                }
                if(diseaseExists)
                    break;
                bucket = bucket->next;
            }
            if(!diseaseExists){
                fprintf(stdout, "The disease %s does not exist in the system\n~$:", disease);
            }
        }else
            fprintf(stdout, "The number of hospitalised patients for %s: 0\n~$:", disease);
    }else{

        HashElement iterator = hashITERATOR(manager->diseaseHashTable);
        while(hashIterateValues(&iterator, COUNT_HOSPITALISED) != NULL);
        fprintf(stdout, "Total number of patients counted: %d\n~$:", iterator.counter);
    }
}

/**
 * Exit from the application - Memory dialloccation
 * */
void exitMonitor(CmdManager* manager){

    fprintf(stdout, "Destroying disease HashTable...\n");
    hashDestroy(manager->diseaseHashTable);

    fprintf(stdout, "Destroying country HashTable...\n");
    hashDestroy(manager->countryHashTable);

    fprintf(stdout, "Destroy patient list...\n");
    listMemoryDeallock(manager->patientList);

    free(manager);

    exit(0);
}


void commandServer(CmdManager* manager){
    char* command = NULL;
    char* line = NULL;

    size_t length = 0;

    fprintf(stdout,"~$:");
    while (getline(&line, &length, stdin) != EOF){

        command = strtok(line, "\n");
        if(command == NULL){
            continue;
        }else if(strcmp(command, "/help") == 0){
            helpDesc();
        } else if(strcmp(command, "/exit") == 0){
            exitMonitor(manager);
        }

        command = strtok(line, " ");

        if(strcmp(command, "/globalDiseaseStats") == 0) {

            char* arg1 = strtok(NULL, " ");
            char* arg2 = strtok(NULL, " ");

            if (arg1 != NULL && arg2 != NULL){

                Date* date1 = malloc(sizeof(struct Date));
                Date* date2 = malloc(sizeof(struct Date));

                date1->day = atoi(strtok(arg1, "-"));
                date1->month = atoi(strtok(NULL, "-"));
                date1->year = atoi(strtok(NULL, "-"));
                date2->day = atoi(strtok(arg2, "-"));
                date2->month = atoi(strtok(NULL, "-"));
                date2->year = atoi(strtok(NULL, "-"));
                globalDiseaseStats(manager, date1, date2);

                free(date1);
                free(date2);

            }else if (arg1 == NULL && arg2 == NULL){
                globalDiseaseStats(manager, 0-0-0, 0-0-0);
            }else if(arg2 == NULL && arg1 != NULL){
                fprintf(stderr, "Missing date2. Please try again...\n~$:");
            }


        } else if(strcmp(command, "/diseaseFrequency") == 0){
            Date* date1;
            Date* date2;
            date1 = malloc(sizeof(struct Date));
            date2 = malloc(sizeof(struct Date));

            char* virusName = strtok(NULL, " ");   //virus
            char* arg2 = strtok(NULL, " ");   //date1
            char* arg3 = strtok(NULL, " ");   //date2
            char* country = strtok(NULL, " ");

            date1->day = atoi(strtok(arg2, "-"));
            date1->month = atoi(strtok(NULL, "-"));
            date1->year = atoi(strtok(NULL, "-"));
            date2->day = atoi(strtok(arg3, "-"));
            date2->month = atoi(strtok(NULL, "-"));
            date2->year = atoi(strtok(NULL, "-"));

            if(country != NULL){
                diseaseFrequency(manager, virusName, date1, date2, country);
            }else
                diseaseFrequency(manager, virusName, date1, date2, NULL);

            free(date1);
            free(date2);

        } else if(strcmp(command, "/topk_Diseases") == 0){

            int k = atoi(strtok(NULL, " "));
            char* country = strtok(NULL, " ");
            char* arg3 = strtok(NULL, " ");
            char* arg4 = strtok(NULL, " ");

            if (arg3 != NULL && arg4 != NULL){
                Date* date1 = malloc(sizeof(struct Date));
                Date* date2 = malloc(sizeof(struct Date));
                date1->day = atoi(strtok(arg3, "-"));
                date1->month = atoi(strtok(NULL, "-"));
                date1->year = atoi(strtok(NULL, "-"));
                date2->day = atoi(strtok(arg4, "-"));
                date2->month = atoi(strtok(NULL, "-"));
                date2->year = atoi(strtok(NULL, "-"));
                topk_Diseases(manager, k, country, date1, date2);
                free(date1);
                free(date2);

            }else if (arg3 == NULL && arg4 == NULL){
                topk_Diseases(manager, k, country, NULL, NULL);
            }else if(arg3 != NULL && arg4 == NULL ){
                fprintf(stderr, "Missing date2. Please try again...\n~$:");
            }

        } else if(strcmp(command, "/topk_Countries") == 0){

            int k = atoi(strtok(NULL, " "));
            char* disease = strtok(NULL, " ");
            char* arg3 = strtok(NULL, " ");
            char* arg4 = strtok(NULL, " ");

            if (arg3 != NULL && arg4 != NULL){
                Date* date1 = malloc(sizeof(struct Date));
                Date* date2 = malloc(sizeof(struct Date));
                date1->day = atoi(strtok(arg3, "-"));
                date1->month = atoi(strtok(NULL, "-"));
                date1->year = atoi(strtok(NULL, "-"));
                date2->day = atoi(strtok(arg4, "-"));
                date2->month = atoi(strtok(NULL, "-"));
                date2->year = atoi(strtok(NULL, "-"));
                topk_Countries(manager, k, disease, date1, date2);

                free(date1);
                free(date2);

            }else if (arg3 == NULL && arg4 == NULL){
                topk_Countries(manager, k, disease, NULL, NULL);
            }else if(arg3 != NULL && arg4 == NULL ){
                fprintf(stderr, "Missing date2. Please try again...\n~$:");
            }
        } else if(strcmp(command, "/recordPatientExit") == 0){
            char* arguments = strtok(NULL, "\n");
            recordPatientExit(manager, arguments);
        } else if(strcmp(command, "/numCurrentPatients") == 0){  //the query is called with the disease argument defined
            char* disease = strtok(NULL, "\n");
            numCurrentPatients(manager, disease);
        } else if(strcmp(command, "/insertPatientRecord") == 0){
            char* arguments = strtok(NULL, "\n");
            insertPatientRecord(manager, arguments);
        } else{
            fprintf(stdout, "The command you have entered does not exist.\n You can see the available commands by hitting /help.\n");
        }

    }
}


/**
 * Help desc for all the actions of the application and how to call them.
 * */
void helpDesc(){
    fprintf(stdout,"/globalDiseaseStats [date1 date2]\n\n"
                   "/diseaseFrequency virusName [country] date1 date2\n\n"
                   "/topk-Diseases k country [date1 date2]\n\n"
                   "/topk-Countries k disease [date1 date2]\n\n"
                   "/insertPatientRecord recordID patientFirstName patientLastName diseaseID country entryDate [exitDate]\n\n"
                   "/recordPatientExit recordID exitDate\n\n"
                   "/numCurrentPatients [disease]\n\n"
                   "/exit\n\n ~$:");
}


