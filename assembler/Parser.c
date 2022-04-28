/* Inclusion */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parser.h"
#include "ErrorHandler.h"

/* Externed variables */
extern char instructions_strings[INSTRUCTION_COUNT][5];
extern char memory_alloc_instruction_strings[ALLOC_INSTRUCTIONS_COUNT][7];
extern FILE * InputFileFD;
extern FILE * OutputFileFD;
extern CodeMetaData_t * FirstLineCode;
extern CodeMetaData_t * CurrentLineCode;
extern CodeMetaData_t * CurrentCodeDecoded;
extern AllocatedSpace_t * MemStack;
extern AllocatedSpace_t * CurrMemLocation;
extern ExternVariable_t * FirstExtern;
extern ExternVariable_t * CurrentExtern;
extern ExternVariable_t * FirstEntry;
extern ExternVariable_t * CurrentEntry;
extern ExternOut_t * FirstExternOut;
extern ExternOut_t * CurrentExternOut;
extern ExternOut_t * FirstEntryOut;
extern ExternOut_t * CurrentEntryOut;
extern char FullFileName[FILE_NAME_SIZE];
extern char FileWithoutExtantion[FILE_NAME_SIZE];
extern char LineText[LINE_SIZE];
extern Error_t * error_list_head;
extern Error_t * error_list_tail;
extern int number_of_errors;
extern int line_number;

/* Functions */
static void ParseLine(char * Line, Instruction_t * instruction);


/* Public functions definition */
void Parser_Init(char * FileName)
{
	strcpy(FullFileName, FileName);

	FirstLineCode = (CodeMetaData_t *)calloc(sizeof(CodeMetaData_t), 1);
	FirstLineCode->Address = 100;

	CurrentLineCode = FirstLineCode;
	CurrentCodeDecoded = FirstLineCode;

	MemStack = (AllocatedSpace_t *)calloc(sizeof(AllocatedSpace_t), 1);
	CurrMemLocation = MemStack;

	FirstExtern = (ExternVariable_t *)calloc(sizeof(ExternVariable_t), 1);
	CurrentExtern = FirstExtern;

	FirstEntry = (ExternVariable_t *)calloc(sizeof(ExternVariable_t), 1);
	CurrentEntry = FirstEntry;

	FirstExternOut = (ExternOut_t *)calloc(sizeof(ExternOut_t), 1);
	CurrentExternOut = FirstExternOut;

	FirstEntryOut = (ExternOut_t *)calloc(sizeof(ExternOut_t), 1);
	CurrentEntryOut = FirstEntryOut;

	strcpy(FileWithoutExtantion, FullFileName);
	FileWithoutExtantion[strlen(FullFileName) - 3] = 0;

	InputFileFD = fopen(FullFileName, "r");

	ErrorHandler_Init();
}

void Parser_GetAllLabels()
{
	char label[LABEL_SIZE] = { 0 };
	int tempData; 
	int LineIndex = 0;
	MemAllocTypes_e funcRet;

	memset(LineText, 0, LINE_SIZE);

	tempData = HelperFunctions_ReadLine(LineText);

	while (tempData != -1)
	{
		LineIndex = 0;
		if (tempData != 0) /* Not an empty Line */
		{
			LineIndex = HelperFunctions_LableInLine(LineText, label);
			LineIndex += (LineIndex != 0);
			if (LineIndex != 0) /* Contains a label */
			{
				LineIndex += HelperFunctions_SkipWhiteSpaces(&LineText[LineIndex]);
				if (LineText[LineIndex] != '.') /* Not a memory allocation instruction */
				{
					strcpy(CurrentLineCode->label, label);
					memset(label, 0, sizeof(label));
					if (LineText[LineIndex] != '\0' && LineText[LineIndex] != '\n' && LineText[LineIndex] != 13)
					{
						LineIndex += HelperFunctions_SkipWhiteSpaces(&LineText[LineIndex]);
						strcpy(CurrentLineCode->code, &LineText[LineIndex]);
					}
					HelperFunctions_AddLine();
				}
				else /* Memory allocation instruction */
				{
					funcRet = MemmoryCommandsHandler_GetMemoryInstruction(&LineText[LineIndex + 1]);
					if (funcRet >= DB && funcRet <= ASCIZ)
					{
						strcpy(CurrMemLocation->label, label);
						strcpy(CurrMemLocation->code, &LineText[LineIndex]);
						MemmoryCommandsHandler_AllocInstruction(&LineText[LineIndex + 1], CurrMemLocation);
						MemmoryCommandsHandler_NewMemoryLine();
					}
					else if (funcRet == EXTERN)
					{
						if (strchr(&LineText[LineIndex], ',') != NULL)
						{
							ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, HelperFunctions_GetLineNumber());
						}
						strcpy(CurrentExtern->label, label);
						strcpy(CurrentExtern->code, &LineText[LineIndex]);

						LineIndex += strlen(memory_alloc_instruction_strings[EXTERN]) + 1;
						LineIndex += HelperFunctions_SkipWhiteSpaces(&LineText[LineIndex]);

						UpdateOutputData_AddExtern(&LineText[LineIndex]);
					}
					else if (funcRet == ENTRY)
					{
						if (strchr(&LineText[LineIndex], ',') != NULL)
						{
							ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, HelperFunctions_GetLineNumber());
						}
						strcpy(CurrentEntry->label, label);
						strcpy(CurrentEntry->code, &LineText[LineIndex]);

						LineIndex += strlen(memory_alloc_instruction_strings[ENTRY]) + 1;
						UpdateOutputData_AddEntry(&LineText[LineIndex]);
					}
				}
			}
			else
			{
				LineIndex += HelperFunctions_SkipWhiteSpaces(&LineText[LineIndex]);
				if(LineText[LineIndex] == '.')
				{
					funcRet = MemmoryCommandsHandler_GetMemoryInstruction(&LineText[LineIndex + 1]);
					if (funcRet >= DB && funcRet <= ASCIZ)
					{
						strcpy(CurrMemLocation->code, &LineText[LineIndex]);
						MemmoryCommandsHandler_AllocInstruction(&LineText[LineIndex + 1], CurrMemLocation);
						MemmoryCommandsHandler_NewMemoryLine();
					}
					else if (funcRet == EXTERN)
					{
						if (strchr(&LineText[LineIndex], ',') != NULL)
						{
							ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, HelperFunctions_GetLineNumber());
						}
						strcpy(CurrentExtern->code, &LineText[LineIndex]);

						LineIndex += strlen(memory_alloc_instruction_strings[EXTERN]) + 1;
						LineIndex += HelperFunctions_SkipWhiteSpaces(&LineText[LineIndex]);

						UpdateOutputData_AddExtern(&LineText[LineIndex]);
					}
					else if (funcRet == ENTRY)
					{
						if (strchr(&LineText[LineIndex], ',') != NULL)
						{
							ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, HelperFunctions_GetLineNumber());
						}
						strcpy(CurrentEntry->code, &LineText[LineIndex]);

						LineIndex += strlen(memory_alloc_instruction_strings[ENTRY]) + 1;
						LineIndex += HelperFunctions_SkipWhiteSpaces(&LineText[LineIndex]);

						UpdateOutputData_AddEntry(&LineText[LineIndex]);
					}
					
				}
				else if (LineText[LineIndex] != '\0' && LineText[LineIndex] != '\n' && LineText[LineIndex] != 13)
				{
					LineIndex += HelperFunctions_SkipWhiteSpaces(&LineText[LineIndex]);
					strcpy(CurrentLineCode->code, &LineText[LineIndex]);
					HelperFunctions_AddLine();
				}
				else
				{
					HelperFunctions_AddLine();
				}
			}
		}
		memset(LineText, 0, sizeof(LineText));
		tempData = HelperFunctions_ReadLine(LineText);
	}
}

void Parser_Parse()
{
	CodeMetaData_t * tempData = FirstLineCode;

	MemmoryCommandsHandler_HandleMemoryAllocation();
	while (tempData->nxtCodeLine != NULL)
	{
		ParseLine(tempData->code, &tempData->instruction_information);
		tempData = tempData->nxtCodeLine;
		CurrentCodeDecoded = CurrentCodeDecoded->nxtCodeLine;
	}
}

static void ParseLine(char * Line, Instruction_t * instruction)
{
	/* Needed variables */
	char text[LABEL_SIZE] = { 0 };
	int Line_index = 0;
	int Text_index = 0;
	int i;

	/* Function logic */
	/* Remove white spaces from the first characters of the line if exists */
	Line_index += HelperFunctions_SkipWhiteSpaces(Line);
getTheInstruction:
	memset(text, 0, LABEL_SIZE);
	Text_index = 0;
	/* Get the instruction name */
	if (Line[Line_index] == '.')
	{
		strcpy(CurrMemLocation->code, &LineText[Line_index]);
		MemmoryCommandsHandler_AllocInstruction(&Line[Line_index + 1], CurrMemLocation);
	}
	else
	{
		while (Line[Line_index] != ' ' && Line[Line_index] != '\n' && Line[Line_index] != '\0' && Line[Line_index] != 13)
		{
			text[Text_index] = Line[Line_index];
			Line_index++;
			Text_index++;
		}
		for (i = 0; i < INSTRUCTION_COUNT; i++)
		{
			if (strcmp(text, instructions_strings[i]) == 0)
			{
				break;
			}
		}
		if (i == INSTRUCTION_COUNT)
		{
			/* Not an instruction */
			/* Check if memory allocation instruction */
			if (text[0] == '.')
			{
				text[Text_index - 1] = 0;
				Line_index += HelperFunctions_SkipWhiteSpaces(&Line[Line_index]);
				strcpy(CurrentLineCode->label, text);
				/* Handle the label - using goto because of a suggestion on stackoverflow that it might help here */
				goto getTheInstruction;

			}
			else /* Not an instruction */
			{
				ErrorHandler_RegisterError(ERROR_NOT_AN_INSTRUCTION, instruction->lineNumber);
			}
		}
		else
		{
			instruction->Instruction_Name = i;
		}

		/* Get the type of instruction */
		if (i <= MVLO)
		{
			/* R-Type */
			instruction->type = R_TYPE;
			ParseMainCommands_R_Instruction(&Line[Line_index], instruction);
		}
		else if (i >= ADDI && i <= SH)
		{
			/* I-Type */
			instruction->type = I_TYPE;
			ParseMainCommands_I_Instruction(Line, instruction, Line_index);
		}
		else if(i >= JMP && i <= STOP)
		{
			/* J-Type */
			instruction->type = J_TYPE;
			ParseMainCommands_J_Instruction(Line, instruction, Line_index);
		}
	}
}

void Parser_GetMachineCode()
{
	CodeMetaData_t * tempData = FirstLineCode;
	while (tempData->nxtCodeLine != NULL)
	{
		HelperFunctions_GenerateMachineCode(tempData);
		tempData = tempData->nxtCodeLine;
	}
}

static void free_mem()
{
	
}

void Parser_destroy()
{
	AllocatedSpace_t * tempMem = MemStack->nxtMemoryAlloc;
	AllocatedSpace_t * toDeleteMem = MemStack;
	CodeMetaData_t * tempInst = FirstLineCode->nxtCodeLine;
	CodeMetaData_t * toDeleteInst = FirstLineCode;
	ExternVariable_t * tempExtern = FirstExtern->nxtVar;
	ExternVariable_t * toDeleteExtern = FirstExtern;
	ExternVariable_t * tempEntry = FirstEntry->nxtVar;
	ExternVariable_t * toDeleteEntry = FirstEntry;
	ExternOut_t * tempExtOut = FirstExternOut->nxt;
	ExternOut_t * toDeleteExtOut = FirstExternOut;
	ExternOut_t * tempEntOut = FirstEntryOut->nxt;
	ExternOut_t * toDeleteEntOut = FirstEntryOut;

	Error_t * tempError = error_list_head->nxt_error;
	Error_t * toDeleteError = error_list_head;

	while (tempMem != NULL)
	{
		free(toDeleteMem);
		toDeleteMem = tempMem;
		tempMem = tempMem->nxtMemoryAlloc;
	}
	free(toDeleteMem);
	
	while (tempInst != NULL)
	{
		free(toDeleteInst);
		toDeleteInst = tempInst;
		tempInst = tempInst->nxtCodeLine;
	}
	free(toDeleteInst);

	while (tempExtern != NULL)
	{
		free(toDeleteExtern);
		toDeleteExtern = tempExtern;
		tempExtern = tempExtern->nxtVar;
	}
	free(toDeleteExtern);
	
	while (tempEntry != NULL)
	{
		free(toDeleteEntry);
		toDeleteEntry = tempEntry;
		tempEntry = tempEntry->nxtVar;
	}
	free(toDeleteEntry);

	while (tempExtOut != NULL)
	{
		free(toDeleteExtOut);
		toDeleteExtOut = tempExtOut;
		tempExtOut = tempExtOut->nxt;
	}
	free(toDeleteExtOut);
	
	while (tempEntOut != NULL)
	{
		free(toDeleteEntOut);
		toDeleteEntOut = tempEntOut;
		tempEntOut = tempEntOut->nxt;
	}
	free(toDeleteEntOut);

	while (toDeleteError->nxt_error != NULL)
	{
		free(toDeleteError);
		toDeleteError = tempError;
		tempError = tempError->nxt_error;
	}
	free(toDeleteError);
	number_of_errors = 0;
	line_number = 0;

	FirstLineCode = NULL;
	CurrentLineCode = NULL;

	MemStack = NULL;
	CurrMemLocation = NULL;

	FirstExtern = NULL;
	CurrentExtern = NULL;

	FirstEntry = NULL;
	CurrentEntry = NULL;

	FirstExternOut = NULL;
	CurrentExternOut = NULL;

	FirstEntryOut = NULL;
	CurrentEntryOut = NULL;

	toDeleteError = NULL;
	tempError = NULL;
}