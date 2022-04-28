#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Parser.h"
#include "ErrorHandler.h"


int main(int argc, char *argv[])
{
	int i;
	
	if(argc < 2) {
		printf("Usage: ./assembler filename.as\n");
		exit(1);
	}

	/* run over all of the arguments and execute methods */
	for (i = 1; i < argc; i++)
	{
		Parser_Init(argv[i]);
		Parser_GetAllLabels();
		Parser_Parse();
		Parser_GetMachineCode();
		if (ErrorHandler_GetErrorsCount() == 0)
		{
			GenerateOutput_exportResults();
		}
		else
		{
			ErrorHandler_DisplayErrorMessages();
		}
		Parser_destroy();
	}
	return 0;
}
