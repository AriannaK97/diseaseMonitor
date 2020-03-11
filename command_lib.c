//
// Created by AriannaK97 on 12/3/20.
//

#include "command_lib.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * For every disease, print the number of the diseased people monitored in the system.
 * If [date1, date2] are defined we get the number of diseases monitored during the period
 * defined form the given dates.
 * Cmd Args: [date1, date2]
 * */
void globalDiseaseStats(){

}

/**
 * If the argument [country] is not define, the application prints for the virusName defined the number of
 * the diseased monitored in the system during the specified period between [date1, date2]. If [country] is
 * defined, the application prints the number of the diseased in this [country] for the specified period.
 * Cmd Args: virusName [country] date1 date2
 * */
void diseaseFrequency(){

}


/**
 * For the specified country the application prints the diseases with the top k number of diseased cases
 * during the period [date1, date2], if that is specified.
 * Cmd Args: k country [date1, date2]
 * */
void topk_Diseases(){

}

/**
 * For the specified disease print the countries with the top k number of cases
 * Cmd Args: k disease [date1, date2]
 * */
void topk_Countries(){

}

/**
 * Insert a new patient record in the system
 * Cmd Args: recordID patientFirstName patientLastName diseaseID entryDate [exitDate]
 * */
void insertPatientRecord(){

}


/**
 * Insert exitDate for the given recordID
 * Cmd Args: recordID exitDate
 * */
void recordPatientDateExit(){

}

/**
 * If the argument [disease] is given, the application prints the number of the patients hospitalised with the
 * specified disease. If the [disease] is not specified the application prints for all the diseases in the system
 * the records of all the patients that are still hospitalised.
 * Cmd Args: [disease]
 * */
void numCurrentPatients(){

}

/**
 * Exit from the application - Memory dialloccation
 * */
void exitMonitor(){

}


