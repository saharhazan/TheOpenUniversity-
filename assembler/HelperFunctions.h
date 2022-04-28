#ifndef HELPER_FUNCTIONS_H_
#define HELPER_FUNCTIONS_H_
/* Inclusion */
#include "SharedHeader.h"
#include <string.h>

/**
 * Read line from the file
 * @param RetLine - line
 */ 
int HelperFunctions_ReadLine(char * RetLine);

/**
 * Check if line contains a label
 * @param line - line
 * @param RetLabel
 */
int HelperFunctions_LableInLine(char * Line, char * RetLabel);

/**
 * function to set a new line of code to the struct
 */ 
void HelperFunctions_AddLine();

/**
 * Get the address of the label
 * @param labelName - the label to get the address of
 */ 
int HelperFunctions_GetLabelAddress(char * labelName);

/**
 * get line address
 * @param linetxt - the line t get the address from
 */ 
int HelperFunctions_GetLineAddress(char * linetxt);

/**
 * helper function to parse until a certain char
 * @param line - line to parse
 * @param stopChar - a char to stop parsing when getting to it
 */ 
int ParseUntil(char * Line, char stopChar);

/**
 * helper function to parse until a number is shown
 * @param line - line to parse
 */ 
int ParseUntilNumber(char * Line);

/**
 * Remove white spaces from a certain line
 * @param Line - line to remoe spaces from
 */ 
int HelperFunctions_SkipWhiteSpaces(char * Line);

/**
 * Get machine code from data ptr(coding)
 * @param DataPtr - the "full coding" to get the machine code from
 */ 
void HelperFunctions_GenerateMachineCode(CodeMetaData_t * DataPtr);

/**
 * Simple function to get line number
 */ 
int HelperFunctions_GetLineNumber();

#endif