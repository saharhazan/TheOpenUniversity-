/* Inclusion */
#include <stdlib.h>
#include "MemoryCommandsHandler.h"
#include "ErrorHandler.h"

/* Externed variables */
extern char memory_alloc_instruction_strings[ALLOC_INSTRUCTIONS_COUNT][7];
extern AllocatedSpace_t * CurrMemLocation;
extern AllocatedSpace_t * MemStack;
extern CodeMetaData_t * FirstLineCode;

static void MemoryGetData(char * instruction, AllocatedSpace_t * AllocData);
static int count_parameters(char * instruction);
static int GetStartAddressForMemory();
static int countAscizData(char * DataTxt);


void MemmoryCommandsHandler_AllocInstruction(char * instruction, AllocatedSpace_t * DataPtr)
{
	char text[8];
	int text_index = 0;
	int i;

	for(text_index = 0; text_index < 8 && instruction[text_index] != ' ' && instruction[text_index] != '\n' && instruction[text_index] != '\0' && instruction[text_index] != 13; text_index++)
	{
		text[text_index] = instruction[text_index];
	}
	text[text_index] = 0;

	for (i = 0; i < ALLOC_INSTRUCTIONS_COUNT; i++)
	{
		if (strcmp(text, memory_alloc_instruction_strings[i]) == 0)
		{
			CurrMemLocation->InstructionType = i;
		}
	}
	
	text_index += HelperFunctions_SkipWhiteSpaces(&instruction[text_index]);
	MemoryGetData(&instruction[text_index], DataPtr);

}

static void MemoryGetData(char * instruction, AllocatedSpace_t * AllocData)
{
	int instruction_index = 0;
	int text_index = 0;
	char text[5];
	int i;
	memset(text, 0, 5);


	if (AllocData->InstructionType >= DB && AllocData->InstructionType <= DW)
	{
		AllocData->Data = calloc(sizeof(AllocatedSpace_t), count_parameters(instruction));
		while (instruction[instruction_index] != '\0' && instruction[instruction_index] != '\n')
		{
			while ((instruction[instruction_index] >= '0' && instruction[instruction_index] <= '9') || (instruction[instruction_index] == '-'))
			{
				text[text_index] = instruction[instruction_index];
				instruction_index++;
				text_index++;
			}
			AllocData->Data[AllocData->count] = atoi(text);
			AllocData->count++;
			memset(text, 0, 5);
			text_index = 0;
			if (instruction[instruction_index] == ',' && 
				(instruction[instruction_index + 1] == '\n' || instruction[instruction_index + 1] == '\0' || instruction[instruction_index + 1] == 13))
			{
				ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, HelperFunctions_GetLineNumber());
				instruction_index++;
			}
			else if (instruction[instruction_index] == ',')
			{
				instruction_index++;
				instruction_index += HelperFunctions_SkipWhiteSpaces(&instruction[instruction_index]);
				if (instruction[instruction_index] == '\n' || instruction[instruction_index] == '\0' || instruction[instruction_index] == 13)
				{
					ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, HelperFunctions_GetLineNumber());
				}
			}
		}

	}
	else if (AllocData->InstructionType == ASCIZ)
	{
		AllocData->count = countAscizData(&instruction[1]) + 1;
		AllocData->Data = calloc(sizeof(AllocatedSpace_t), AllocData->count);
		
		for (i = 0; i < AllocData->count - 1; i++)
		{
			AllocData->Data[i] = instruction[i + 1];
		}
		AllocData->Data[AllocData->count - 1] = 0;
	}
}

static int count_parameters(char * instruction)
{
	int count = 0;
	int i = 0;

	for (i = 0; instruction[i] != '\n' && instruction[i] != '\0' && instruction[i] != 13; i++)
	{
		count += (instruction[i] == ',');
	}

	return count + 1;
}

void MemmoryCommandsHandler_NewMemoryLine()
{
	CurrMemLocation->nxtMemoryAlloc = (AllocatedSpace_t *)calloc(sizeof(AllocatedSpace_t), 1);
	CurrMemLocation = CurrMemLocation->nxtMemoryAlloc;
}

void MemmoryCommandsHandler_HandleMemoryAllocation()
{
	AllocatedSpace_t * curr_stack_p = MemStack;
	int address;

	address = GetStartAddressForMemory();
	while (curr_stack_p->nxtMemoryAlloc != NULL)
	{
		curr_stack_p->Address = address;
		switch (curr_stack_p->InstructionType)
		{
		case DB:
			address += 1 * curr_stack_p->count;
			break;
		case DH:
			address += 2 * curr_stack_p->count;
			break;
		case DW:
			address += 4 * curr_stack_p->count;
			break;
		case ASCIZ:
			address += 1 * curr_stack_p->count;
			break;
		default:
			break;
		}
		curr_stack_p = curr_stack_p->nxtMemoryAlloc;
	}
}

static int GetStartAddressForMemory()
{
	CodeMetaData_t * temp = FirstLineCode;

	while (temp->nxtCodeLine != NULL)
	{
		temp = temp->nxtCodeLine;
	}

	return temp->Address - 4 < 100 ? 100 : temp->Address;
}

MemAllocTypes_e MemmoryCommandsHandler_GetMemoryInstruction(char * instructionText)
{
	char text[7] = { 0 };
	int i = 0;

	while (instructionText[i] != ' ' && instructionText[i] != '\t' &&
		instructionText[i] != '\n' && instructionText[i] != '\0' && instructionText[i] != 13)
	{
		text[i] = instructionText[i];
		i++;
	}
	text[i] = 0;

	for (i = 0; i < ALLOC_INSTRUCTIONS_COUNT; i++)
	{
		if (strcmp(text, memory_alloc_instruction_strings[i]) == 0)
		{
			break;
		}
	}
	if (i == ALLOC_INSTRUCTIONS_COUNT) /* Not an instruction */
	{
		ErrorHandler_RegisterError(ERROR_NOT_AN_INSTRUCTION, HelperFunctions_GetLineNumber());
	}
	return i;
}

static int countAscizData(char * DataTxt)
{
	int index = 0;
	int temp_index;

	while (DataTxt[index] != '\'' && DataTxt[index] != '\"')
	{
		index++;
	}

	temp_index = index + 1 + HelperFunctions_SkipWhiteSpaces(&DataTxt[index]);
	if (DataTxt[temp_index] == ',')
	{
		ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, HelperFunctions_GetLineNumber());
	}

	return index;
}