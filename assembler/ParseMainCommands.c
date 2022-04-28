/* Inclusion */
#include <stdlib.h>
#include <string.h>
#include "ParseMainCommands.h"
#include "HelperFunctions.h"
#include "ErrorHandler.h"

static int getRegister(char * CodeLine,int *Line_index, Instruction_t * instruction);
static int getLabel(char *CodeLine, int *Line_index, Instruction_t *instruction);



static int getRegister(char * CodeLine,int *Line_index, Instruction_t * instruction)
{
	char text[5] = { 0 };
	int Text_index = 0;
	memset(text, 0, sizeof(text));

	if (CodeLine[*Line_index] != '$')
	{
		ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
		return -1;
	}
	(*Line_index)++;
	while (Text_index < 5 && CodeLine[*Line_index] >= '0' && CodeLine[*Line_index] <= '9') /* Stil have numbers */
	{
		text[Text_index] = CodeLine[*Line_index];
		(*Line_index)++;
		Text_index++;
	}
	if (Text_index == 0)
	{
		ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
		return -1;
	}
	if(atoi(text) >= 32)
	{
		ErrorHandler_RegisterError(ERROR_NOT_A_REGISTER, instruction->lineNumber);
		return -1;
	}
	else
	{
		return atoi(text);
	}
}

static int getLabel(char *CodeLine, int *Line_index, Instruction_t *instruction)
{
	char text[LABEL_SIZE];
	int Text_index = 0;
	memset(text, 0, LABEL_SIZE);
	while (Text_index  < LABEL_SIZE && CodeLine[*Line_index] != ' ' && CodeLine[*Line_index] != '\t' && CodeLine[*Line_index] != '\n' && CodeLine[*Line_index] != '\0')
	{
		text[Text_index] = CodeLine[*Line_index];
		Text_index++;
		(*Line_index)++;
	}
	if (Text_index == 0)
	{
		ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
		return -2;
	}

	*Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + *Line_index));
	if (CodeLine[*Line_index] != '\n' && CodeLine[*Line_index] != '\0')
	{
		ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
		return -2;
	}
	return HelperFunctions_GetLabelAddress(text);
}

static int getImmidiate(char *CodeLine, int *Line_index, Instruction_t *instruction)
{
	char text[LABEL_SIZE];
	int Text_index = 0;
	memset(text, 0, sizeof(text));

	/* Extract immidiate value */
	if (CodeLine[*Line_index] == '-')
	{
		text[Text_index] = '-';
		Text_index++;
		(*Line_index)++;
	}
	if (CodeLine[*Line_index] >= '0' && CodeLine[*Line_index] <= '9')
	{
		while (Text_index < LABEL_SIZE && CodeLine[*Line_index] >= '0' && CodeLine[*Line_index] <= '9') /* Stil have numbers */
		{
			text[Text_index] = CodeLine[*Line_index];
			(*Line_index)++;
			Text_index++;
		}
		if ((text[0] == '-' && text[1] == '\0') || text[0] == '\0')
		{
			ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
			return 10*LABEL_SIZE + 1; /*since the biggest number cant be more LABEL_SIZE digits long thid number ant be user input thus can be used for indicating an error*/
		}

		return atoi(text);
	}
	else
	{
		ErrorHandler_RegisterError(ERROR_IMMIDIATE_VALUE, instruction->lineNumber);
		return 10*LABEL_SIZE + 1;
	}

}

void ParseMainCommands_R_Instruction(char * CodeLine, Instruction_t * instruction)
{
	/* Needed variables */
	int Line_index = 0;

	/* Get the first parameter of the line */
	/* Prepare for extracting the first parameter */
	Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));
	
	instruction->RS = getRegister(CodeLine, &Line_index, instruction);

	if (instruction->Instruction_Name < MOVE || instruction->Instruction_Name > MVLO)
	{
		/* Prepare for extracting RT */

		/* RT Parameter */
		Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));
		if (CodeLine[Line_index] != ',')
		{
			ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
			return;
		}
		Line_index++;

		Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));
		
		instruction->RT = getRegister(CodeLine,&Line_index,instruction);
		
	}
	else
	{
		instruction->RT = 0;
	}

	/* Extracting RD */
	Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));
	if (CodeLine[Line_index] != ',')
	{
		ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
		return;
	}
	Line_index++;

	Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));
	
	instruction->RD = getRegister(CodeLine,&Line_index,instruction);

	Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));
	if (CodeLine[Line_index] != '\n' && CodeLine[Line_index] != '\0')
	{
		ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
		return;
	}

	instruction->function = (instruction->Instruction_Name % 5) + 1;
	instruction->opCode = instruction->Instruction_Name / 5;
}

void ParseMainCommands_I_Instruction(char * CodeLine, Instruction_t * instruction, int index)
{
	/* Needed variables */
	int Line_index = index;
	int currentAddress;
	int targetAddress;

	/* Get the first parameter of the line */
	/* Prepare for extracting the first parameter */
	Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));

	instruction->RS = getRegister(CodeLine,&Line_index,instruction);
	instruction->opCode = 10 + instruction->Instruction_Name - ADDI;

	if (instruction->Instruction_Name >= BNE && instruction->Instruction_Name <= BGT) /* Branching instructions */
	{

		/* RT Parameter */
		Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));
		if (CodeLine[Line_index] != ',')
		{
			ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
			return;
		}
		Line_index++;
		
		Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index)); 
		
		instruction->RT = getRegister(CodeLine,&Line_index,instruction);

		/* get the label */
		Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));
		if (CodeLine[Line_index] != ',')
		{
			ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
			return;
		}
		Line_index++;
		Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));
		if (strchr((CodeLine + Line_index), ',') != NULL)
		{
			ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
			return;
		}
	
		targetAddress = getLabel(CodeLine,&Line_index,instruction);
		currentAddress = HelperFunctions_GetLineAddress(CodeLine);
		if (targetAddress == -1)
		{
			/* Not exit */
			ErrorHandler_RegisterError(ERROR_NOT_A_LABEL, instruction->lineNumber);
			return;
		}
		else
		{
			instruction->immidiateValue = targetAddress - currentAddress;
		}
	}
	else
	{	/* Immidiate value instruction */
		/* Prepare for immidiate value */
		Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));
		if (CodeLine[Line_index] != ',')
		{
			ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
			return;
		}
		Line_index ++;
		Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));

		instruction->immidiateValue = getImmidiate(CodeLine,&Line_index,instruction);
		

		/* prepare for RT Parameter */
		Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));
		if (CodeLine[Line_index] != ',')
		{
			ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
			return;
		}
		Line_index++;
		Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));

		instruction->RT = getRegister(CodeLine,&Line_index,instruction);

		Line_index += HelperFunctions_SkipWhiteSpaces((CodeLine + Line_index));
		if (CodeLine[Line_index] != '\n' && CodeLine[Line_index] != '\0')
		{
			ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
			return;
		}
	}
}

void ParseMainCommands_J_Instruction(char * CodeLine, Instruction_t * instruction, int index)
{
	int index_line = index;
	char text[3] = { 0 };
	int textIndex = 0;

	if (instruction->Instruction_Name == STOP)
	{
		instruction->opCode = 63;
		instruction->Address = 0;
		instruction->Reg = 0;
	}
	else
	{
		instruction->opCode = 30 + (instruction->Instruction_Name - JMP);
		index_line += HelperFunctions_SkipWhiteSpaces(&CodeLine[index_line]);

		if (CodeLine[index_line] == '$')
		{
			index_line++;
			instruction->Reg = 1;

			memset(text, 0, 3);
			while (textIndex < 3 && CodeLine[index_line] != ' ' && CodeLine[index_line] != '\n' && CodeLine[index_line] != '\0')
			{
				text[textIndex] = CodeLine[index_line];
				textIndex++;
				index_line++;
			}
			if (textIndex == 0)
			{
				ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
				return;
			}
			instruction->Address = atoi(text);
			if (instruction->Address >= 32)
			{
				ErrorHandler_RegisterError(ERROR_NOT_A_REGISTER, instruction->lineNumber);
				return;
			}
			index_line += HelperFunctions_SkipWhiteSpaces(&CodeLine[index_line]);
			if (CodeLine[index_line] != '\n' && CodeLine[index_line] != '\0')
			{
				ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
				return;
			}
		}
		else
		{
			instruction->Reg = 0;
			instruction->Address = HelperFunctions_GetLabelAddress(&CodeLine[index_line]);
			if (instruction->Address == -1) /* Not a label */
			{
				ErrorHandler_RegisterError(ERROR_SYNTAX_ERROR, instruction->lineNumber);
				return;
			}
		}
	}
}
