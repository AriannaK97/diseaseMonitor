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
#include "structs.h"


enum defAttribute{
    LINE_LENGTH,
    BUFFER_SIZE,
};

FILE* openFile(char *inputFile);
int getMaxFromFile(FILE* patientRecordsFile, int returnVal);

CmdManager* read_input_file(FILE* patientRecordsFile, size_t maxStrLength, int diseaseHashtableNumOfEntries,
                            int countryHashTableNumOfEntries, size_t bucketSize);

bool dateInputValidation(Date* entryDate, Date* exitDate);

#endif //DISEASEMONITOR_DATA_IO_H
