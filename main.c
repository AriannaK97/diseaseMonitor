#include <stdio.h>
#include "data_io.h"
#include "command_lib.h"

int main(int argc, char** argv) {


/*****************************************************************************
 *                       Handling command line arguments                     *
 *****************************************************************************/
    InputArguments* arguments = getInputArgs(argc, argv);
/*****************************************************************************
 *                       Handling input files                                *
 *****************************************************************************/
    FILE *patientRecordsFile;
    patientRecordsFile = openFile(arguments->inputFile);
    CmdManager* cmdManager;

    cmdManager = read_input_file(patientRecordsFile, getMaxFromFile(patientRecordsFile, LINE_LENGTH),
            arguments->diseaseHashtableNumOfEntries,arguments->countryHashTableNumOfEntries, arguments->bucketSize);

    printList(cmdManager->patientList);
    printHashTable(cmdManager->diseaseHashTable);
    printHashTable(cmdManager->countryHashTable);

    commandServer(cmdManager);

    return 0;
}