#include <stdio.h>
#include "../header/data_io.h"
#include "../header/command_lib.h"

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

    free(arguments);
    fclose(patientRecordsFile);
    //printList(cmdManager->patientList);
    applyOperationOnHashTable(cmdManager->diseaseHashTable, PRINT);
    applyOperationOnHashTable(cmdManager->countryHashTable, PRINT);

    commandServer(cmdManager);

    return 0;
}