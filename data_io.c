//
// Created by AriannaK97 on 9/3/20.
//
#include <stdio.h>
#include <stdlib.h>
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
}
