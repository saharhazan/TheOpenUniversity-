/* Inclusion */
#include <stdlib.h>
#include <stdio.h>
#include "HelperFunctions.h"
#include "UpdateOutputData.h"

extern CodeMetaData_t* FirstLineCode;
extern AllocatedSpace_t * MemStack;
extern ExternVariable_t * FirstExtern;
extern ExternVariable_t * FirstEntry;
extern CodeMetaData_t * CurrentCodeDecoded;
extern FILE * InputFileFD;
extern CodeMetaData_t * CurrentLineCode;
extern int line_number;

int HelperFunctions_ReadLine(char * p_line)
{
	int index = 0;
	char data = 0;

	fscanf(InputFileFD, "%c", &data);
	line_number++;
	if (data == EOF || data == '\0')
	{
		return -1;
	}
	while(data != '\n' && data != EOF && data != '\0' && data != 13 && index < LINE_SIZE)
	{
		p_line[index] = data;
		index++;
		fscanf(InputFileFD, "%c", &data);
	}
	return index;
}

int HelperFunctions_LableInLine(char * Line, char * RetLabel)
{
	int index = 0;
	for(index = 0; index < LINE_SIZE && Line[index] != ' '; index++)
	{
		if (Line[index] == ':') 
		{
			memset(RetLabel,0,LABEL_SIZE);
			strncpy(RetLabel, Line, index);
			return index;
		}
	}
	return 0;
}

void HelperFunctions_AddLine()
{
	CurrentLineCode->nxtCodeLine = (CodeMetaData_t *)calloc(sizeof(CodeMetaData_t), 1);
	CurrentLineCode->nxtCodeLine->Address = CurrentLineCode->Address + 4;
	CurrentLineCode->instruction_information.lineNumber = HelperFunctions_GetLineNumber();
	CurrentLineCode = CurrentLineCode->nxtCodeLine;
}

int HelperFunctions_GetLabelAddress(char * labelName)
{
	CodeMetaData_t * curr_line = FirstLineCode;
	AllocatedSpace_t * curr_mem_alloc_data = MemStack;
	ExternVariable_t * curr_extern = FirstExtern;
	ExternVariable_t * curr_entry = FirstEntry;



	while (curr_line->nxtCodeLine != NULL)
	{
		if (strcmp(labelName, curr_line->label) == 0)
		{
			while (curr_entry->nxtVar != NULL)
			{
				if (strcmp(labelName, curr_entry->varName) == 0)
				{
					UpdateOutputData_AddEntryOut(curr_entry->varName, curr_line->Address);
					break;
				}
				curr_entry = curr_entry->nxtVar;
			}
			return curr_line->Address;
		}
		curr_line = curr_line->nxtCodeLine;
	}

	while (curr_mem_alloc_data->nxtMemoryAlloc != NULL)
	{
		if (strcmp(labelName, curr_mem_alloc_data->label) == 0)
		{
			while (curr_entry->nxtVar != NULL)
			{
				if (strcmp(labelName, curr_entry->varName) == 0)
				{
					UpdateOutputData_AddEntryOut(curr_entry->varName, curr_mem_alloc_data->Address);
					break;
				}
				curr_entry = curr_entry->nxtVar;
			}
			return curr_mem_alloc_data->Address;
		}
		curr_mem_alloc_data = curr_mem_alloc_data->nxtMemoryAlloc;
	}

	while (curr_extern->nxtVar != NULL)
	{
		if (strcmp(labelName, curr_extern->varName) == 0)
		{
			UpdateOutputData_AddExternOut(curr_extern->varName, CurrentCodeDecoded->Address);
			return 0;
		}
		curr_extern = curr_extern->nxtVar;
	}

	return -1;
}

int HelperFunctions_GetLineAddress(char * linetxt)
{
	CodeMetaData_t * curr_line = FirstLineCode;
	while (curr_line->nxtCodeLine != NULL)
	{
		if (strcmp(linetxt, curr_line->code) == 0)
		{
			return curr_line->Address;
		}
		curr_line = curr_line->nxtCodeLine;
	}

	return -1;
}

int HelperFunctions_SkipWhiteSpaces(char * Line)
{
	int index = 0;
	while (Line[index] == ' ' || Line[index] == '\t')
	{
		index++;
	}

	return index;
}

void HelperFunctions_GenerateMachineCode(CodeMetaData_t * Code)
{
	switch (Code->instruction_information.type)
	{
	case R_TYPE:
		Code->Machine_Code = (Code->instruction_information.opCode << 26) |
			(Code->instruction_information.RS << 21) |
			(Code->instruction_information.RT << 16) |
			(Code->instruction_information.RD << 11) |
			(Code->instruction_information.function << 6);
		break;
	case I_TYPE:
		Code->Machine_Code = (Code->instruction_information.opCode << 26) |
			(Code->instruction_information.RS << 21) |
			(Code->instruction_information.RT << 16) |
			((Code->instruction_information.immidiateValue & 0xFFFF));
		break;
	case J_TYPE:
		if (Code->instruction_information.Instruction_Name == STOP)
		{
			Code->Machine_Code = (Code->instruction_information.opCode << 26);
		}
		else
		{
			Code->Machine_Code = (Code->instruction_information.opCode << 26) |
				(((Code->instruction_information.Reg & 0x1) << 25)) |
				((Code->instruction_information.Address) & 0xFFFFFF);
		}
		break;
	default:
		break;
	}
}

int HelperFunctions_GetLineNumber()
{
	return line_number;
}
