//
// Created by AriannaK97 on 12/3/20.
//

#ifndef DISEASEMONITOR_COMMAND_LIB_H
#define DISEASEMONITOR_COMMAND_LIB_H

typedef struct CmdManager CmdManager;

void globalDiseaseStats();

void diseaseFrequency();

void topk_Diseases();

void topk_Countries();

void insertPatientRecord();

void recordPatientDateExit();

void numCurrentPatients();

void exitMonitor(CmdManager* manager);

void commandServer(CmdManager* manager);

void helpDesc();

#endif //DISEASEMONITOR_COMMAND_LIB_H
