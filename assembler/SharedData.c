/* Inclusion */
#include "SharedHeader.h"
#include <stdio.h>

/* Shared Data */
char instructions_strings[INSTRUCTION_COUNT][5] = {
	"add\0\0",
	"sub\0\0",
	"and\0\0",
	"or\0\0\0",
	"nor\0\0",
	"move\0",
	"mvhi\0",
	"mvlo\0",
	"addi\0",
	"subi\0",
	"andi\0",
	"ori\0\0",
	"nori\0",
	"bne\0\0",
	"beq\0\0",
	"blt\0\0",
	"bgt\0\0",
	"lb\0\0\0",
	"sb\0\0\0",
	"lw\0\0\0",
	"sw\0\0\0",
	"lh\0\0\0",
	"sh\0\0\0",
	"jmp\0\0",
	"la\0\0\0",
	"call\0",
	"stop"
};
char memory_alloc_instruction_strings[ALLOC_INSTRUCTIONS_COUNT][7] = {
	"db\0",
	"dh\0",
	"dw\0",
	"asciz\0",
	"entry\0",
	"extern\0"
};
CodeMetaData_t * FirstLineCode;
CodeMetaData_t * CurrentLineCode;
CodeMetaData_t * CurrentCodeDecoded;
FILE * InputFileFD = NULL;
FILE * OutputFileFD = NULL;
char LineText[LINE_SIZE] = { 0 };
char FullFileName[FILE_NAME_SIZE] = { 0 };
char FileWithoutExtantion[FILE_NAME_SIZE] = { 0 };
AllocatedSpace_t * MemStack;
AllocatedSpace_t * CurrMemLocation;
ExternVariable_t * FirstExtern;
ExternVariable_t * CurrentExtern;
ExternVariable_t * FirstEntry;
ExternVariable_t * CurrentEntry;
ExternOut_t * FirstExternOut;
ExternOut_t * CurrentExternOut;
ExternOut_t * FirstEntryOut;
ExternOut_t * CurrentEntryOut;
int line_number = 0;