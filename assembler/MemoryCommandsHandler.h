#ifndef MEMORY_COMMANDS_HANDLER_H_
#define MEMORY_COMMANDS_HANDLER_H_
/* Inclusion */
#include "Configurations.h"
#include "HelperFunctions.h"
#include "UpdateOutputData.h"
#include "ParseMainCommands.h"
#include "MemoryCommandsHandler.h"
#include "GenerateOutput.h"
#include "SharedHeader.h"
#include "HelperFunctions.h"

/**
 * 
 * @param instruction - get the instruction
 * @param DataPtr - Data pointer
 */ 
void MemmoryCommandsHandler_AllocInstruction(char * instruction, AllocatedSpace_t * DataPtr);

/**
 * allocate and assign a new memory line
 */ 
void MemmoryCommandsHandler_NewMemoryLine();

/**
 * Handle memory allocation
 */ 
void MemmoryCommandsHandler_HandleMemoryAllocation();


MemAllocTypes_e MemmoryCommandsHandler_GetMemoryInstruction(char * instructionText);

#endif
