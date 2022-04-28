assembler: assembler.o Parser.o HelperFunctions.o GenerateOutput.o MemoryCommandsHandler.o ParseMainCommands.o SharedData.o UpdateOutputData.o ErrorHandler.o
	gcc -g -Wall -ansi -pedantic assembler.o Parser.o HelperFunctions.o GenerateOutput.o MemoryCommandsHandler.o ParseMainCommands.o SharedData.o UpdateOutputData.o ErrorHandler.o -o assembler

assembler.o: assembler.c Parser.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o

Parser.o: Parser.c Parser.h
	gcc -c -Wall -ansi -pedantic Parser.c -o Parser.o

HelperFunctions.o: HelperFunctions.c HelperFunctions.h
	gcc -c -Wall -ansi -pedantic HelperFunctions.c -o HelperFunctions.o

GenerateOutput.o: GenerateOutput.c GenerateOutput.h
	gcc -c -Wall -ansi -pedantic GenerateOutput.c -o GenerateOutput.o

MemoryCommandsHandler.o: MemoryCommandsHandler.c MemoryCommandsHandler.h
	gcc -c -Wall -ansi -pedantic MemoryCommandsHandler.c -o MemoryCommandsHandler.o

ParseMainCommands.o: ParseMainCommands.c ParseMainCommands.h
	gcc -c -Wall -ansi -pedantic ParseMainCommands.c -o ParseMainCommands.o

SharedData.o: SharedData.c SharedHeader.h
	gcc -c -Wall -ansi -pedantic SharedData.c -o SharedData.o

UpdateOutputData.o: UpdateOutputData.c UpdateOutputData.h
	gcc -c -Wall -ansi -pedantic UpdateOutputData.c -o UpdateOutputData.o

ErrorHandler.o: ErrorHandler.c ErrorHandler.h
	gcc -c -Wall -ansi -pedantic ErrorHandler.c -o ErrorHandler.o
