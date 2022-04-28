#ifndef PARSE_MAIN_COMMANDS_H_
#define PARSE_MAIN_COMMANDS_H_
/* Inclusion */
#include "SharedHeader.h"

/**
 * Parse an R Instruction
 */ 
void ParseMainCommands_R_Instruction(char * Line, Instruction_t * instruction);

/**
 * Parse an I Instruction
 */ 
void ParseMainCommands_I_Instruction(char * Line, Instruction_t * instruction, int index);

/**
 * Parse an J Instruction
 */ 
void ParseMainCommands_J_Instruction(char * Line, Instruction_t * instruction, int index);
#endif