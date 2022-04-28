/* Inclusion */
#include "UpdateOutputData.h"
#include <stdlib.h>
#include <string.h>

/* Externed variables */
extern ExternVariable_t * CurrentExtern;
extern ExternVariable_t * CurrentEntry;
extern ExternOut_t * CurrentExternOut;
extern ExternOut_t * CurrentEntryOut;

void UpdateOutputData_AddExtern(char * var)
{
	memset(CurrentExtern->varName, 0, sizeof(CurrentExtern->varName));
	strcpy(CurrentExtern->varName, var);

	CurrentExtern->nxtVar = (ExternVariable_t *)calloc(sizeof(ExternVariable_t), 1);

	CurrentExtern = CurrentExtern->nxtVar;
}

void UpdateOutputData_AddEntry(char * var)
{
	memset(CurrentEntry->varName, 0, sizeof(CurrentEntry->varName));
	strcpy(CurrentEntry->varName, var);

	CurrentEntry->nxtVar = (ExternVariable_t *)calloc(sizeof(ExternVariable_t), 1);

	CurrentEntry = CurrentEntry->nxtVar;
}

void UpdateOutputData_AddExternOut(char * var, signed int address)
{
	CurrentExternOut->Address = address;
	strcpy(CurrentExternOut->varName, var);

	CurrentExternOut->nxt = (ExternOut_t*)calloc(sizeof(ExternOut_t), 1);

	CurrentExternOut = CurrentExternOut->nxt;
}

void UpdateOutputData_AddEntryOut(char * var, signed int address)
{
	CurrentEntryOut->Address = address;
	strcpy(CurrentEntryOut->varName, var);

	CurrentEntryOut->nxt = (ExternOut_t*)calloc(sizeof(ExternOut_t), 1);

	CurrentEntryOut = CurrentEntryOut->nxt;
}
