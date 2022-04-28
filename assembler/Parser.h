#ifndef PARSER_H_
#define PARSER_H_
/* Inclusion */
#include <stdint.h>
#include "SharedHeader.h"
#include "Configurations.h"
#include "HelperFunctions.h"
#include "UpdateOutputData.h"
#include "ParseMainCommands.h"
#include "MemoryCommandsHandler.h"
#include "GenerateOutput.h"

/**
 * Initialize parsing (variables, callocs and more)
 * @param FileName - file
 */ 
void Parser_Init(char * FileName);

/**
 * Get all labels in file ("first process")
 */ 
void Parser_GetAllLabels();

/**
 * Parse all lines ("second process")
 */ 
void Parser_Parse();

/**
 * Get machine code
 */ 
void Parser_GetMachineCode();

/**
 * Free allocated memory of everything in our program
 */ 
void Parser_destroy();
#endif
