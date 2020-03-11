#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include "data_io.h"
#include "list_lib.h"


int main(int argc, char** argv) {

    size_t bucketSize;
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
            bucketSize = atoi(argv[i + 1]);
            numOfArgs += 2;
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
    size_t BUFFERSIZE;
    List *patientList;

    lineLenght = getMaxFromFile(patientRecordsFile, LINE_LENGTH);
    BUFFERSIZE = getMaxFromFile(patientRecordsFile, BUFFER_SIZE);

    printf("%lu\n", lineLenght);
    printf("%lu\n", BUFFERSIZE);

    patientList = read_input_file(patientRecordsFile, lineLenght);


    return 0;
}