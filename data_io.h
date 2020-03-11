//
// Created by AriannaK97 on 9/3/20.
//

#ifndef DISEASEMONITOR_DATA_IO_H
#define DISEASEMONITOR_DATA_IO_H

#include "list_lib.h"
#include "data_io.h"

struct Date{
    int day;
    int month;
    int year;
};

typedef struct PatientCase{
    int caseNum;
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

struct list* read_input_file(FILE* patientRecordsFile, size_t maxStrLength);
#endif //DISEASEMONITOR_DATA_IO_H
