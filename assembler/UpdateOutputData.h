#ifndef UPDATE_OUTPUT_DATA_H_
#define UPDATE_OUTPUT_DATA_H_
/* Inclusion */
#include "SharedHeader.h"


/**
 * Add extern variable
 * @param var - variable
 */ 
void UpdateOutputData_AddExtern(char * var);

/**
 * Add entry variable
 * @param var - variable
 */ 
void UpdateOutputData_AddEntry(char * var);

/**
 * Add extern out variable
 * @param var - variable
 */ 
void UpdateOutputData_AddExternOut(char * var, signed int address);

/**
 * Add entry out variable
 * @param var - variable
 */ 
void UpdateOutputData_AddEntryOut(char * var, signed int address);
#endif