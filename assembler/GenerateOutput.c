/* Inclusion */
#include <stdio.h>
#include <stdlib.h>
#include "GenerateOutput.h"

extern FILE * OutputFileFD;
extern CodeMetaData_t * FirstLineCode;
extern char FileWithoutExtantion[FILE_NAME_SIZE];
extern ExternOut_t * FirstExternOut;
extern AllocatedSpace_t * MemStack;
extern ExternOut_t * FirstEntryOut;


/**
 * Display the memory data according to the right instruction formatting
 * @param data - the memory data
 */ 
static void printBinary(CodeMetaData_t * data);

/**
 * tries to dump the memory and write it with OutMemoryData
 * upon success will zero the BytesCount and add 4 to the address
 * @param BytesCount - a pointer to BytesCount
 * @param addr - a pointer to current address
 * @param OutData - the data to dump upon hitting 4 bytes
 */
static void tryDumpMem(int* BytesCount, signed int* addr,char* OutData);

/**
 * Display the memory data according to the right instruction formatting
 * @param data - the memory data
 */ 
static void exportMemoryData(AllocatedSpace_t * Data);

/**
 * helper function to print the memory data
 * @param address - the memoery data address
 * @param data - the data itself
 */ 
static void OutMemoryData(signed int address, char * Data);

/**
 * print external stuff (with the right formatting for external)
 */ 
static void exportExtern();

/**
 * print entry stuff (with the right formatting for entry)
 */ 
static void exportEntry();


void GenerateOutput_exportResults()
{
	CodeMetaData_t * temp = FirstLineCode;

	/* .ob file */
	
	sprintf(FileWithoutExtantion, "%s.ob", FileWithoutExtantion);
	OutputFileFD = fopen(FileWithoutExtantion, "w");

	while (temp->nxtCodeLine != NULL)
	{
		printBinary(temp);
		temp = temp->nxtCodeLine;
	}

	exportMemoryData(MemStack);

	fclose(OutputFileFD);
	FileWithoutExtantion[strlen(FileWithoutExtantion) - 3] = 0;

	/* .ext file */
	sprintf(FileWithoutExtantion, "%s.ext", FileWithoutExtantion);
	OutputFileFD = fopen(FileWithoutExtantion, "w");
	exportExtern();
	fclose(OutputFileFD);
	FileWithoutExtantion[strlen(FileWithoutExtantion) - 4] = 0;

	/* .ent file */
	sprintf(FileWithoutExtantion, "%s.ent", FileWithoutExtantion);
	OutputFileFD = fopen(FileWithoutExtantion, "w");
	exportEntry();
	fclose(OutputFileFD);
	FileWithoutExtantion[strlen(FileWithoutExtantion) - 4] = 0;
}




/*Private functions*/
static void printBinary(CodeMetaData_t * data)
{
	fprintf(OutputFileFD, "%04d %02X %02X %02X %02X\n",
				data->Address,
				(unsigned int)(data->Machine_Code & 0xFF), 
				(unsigned int)((data->Machine_Code >> 8) & 0xFF),
				(unsigned int)((data->Machine_Code >> 16) & 0xFF),
			 	(unsigned int)((data->Machine_Code >> 24) & 0xFF));

}

static void tryDumpMem(int* BytesCount, signed int* addr,char* OutData)
{
	if (addr != NULL && BytesCount != NULL && *BytesCount == 4)
	{
		OutMemoryData(*addr, OutData);
		(*addr) += 4;
		(*BytesCount) = 0;
	}
}

static void exportMemoryData(AllocatedSpace_t * Data)
{
	int BytesCount = 0;
	char OutData[4];
	int i, j, maxShift = 0;
	AllocatedSpace_t * temp = Data;
	signed int address = temp->Address;

	while (temp->nxtMemoryAlloc != NULL)
	{
		if (temp->InstructionType == DB || temp->InstructionType == ASCIZ)
		{
			maxShift = 0;
		}
		else if (temp->InstructionType == DH)
		{
			maxShift = 8;
		}
		else if (temp->InstructionType == DW)
		{
			maxShift = 24;
		}

		for (i = 0; i < temp->count; i++){
			tryDumpMem(&BytesCount,&address,OutData);
			for(j = 0; j <= maxShift; j += 8)
			{
				OutData[BytesCount] = (unsigned char)(temp->Data[i] >> j & 0xFF);
				BytesCount++;
				tryDumpMem(&BytesCount,&address,OutData);
			}
		}
		temp = temp->nxtMemoryAlloc;
	}



	if (BytesCount == 1)
	{
		fprintf(OutputFileFD, "%04d %02X\n", address,
			(OutData[0] & 0xFF));
	}
	else if (BytesCount == 2)
	{
		fprintf(OutputFileFD, "%04d %02X %02X\n", address,
			(OutData[0] & 0xFF), (OutData[1] & 0xFF));
	}
	else if (BytesCount == 3)
	{
		fprintf(OutputFileFD, "%04d %02X %02X %02X\n", address,
			(OutData[0] & 0xFF), (OutData[1] & 0xFF),
			(OutData[2] & 0xFF));
	}
	else if (BytesCount == 4)
	{
		OutMemoryData(address, OutData);
	}
}

static void OutMemoryData(signed int address, char * Data)
{
	fprintf(OutputFileFD, "%04d %02X %02X %02X %02X\n",
		address,
		(Data[0] & 0xFF), (Data[1] & 0xFF),
		(Data[2] & 0xFF), (Data[3] & 0xFF));
}


static void exportExtern()
{
	ExternOut_t * temp = FirstExternOut;
	char OutData[LABEL_SIZE + 6];

	while (temp->nxt != NULL)
	{
		sprintf(OutData, "%s %04d\n", temp->varName, temp->Address);
		fprintf(OutputFileFD, "%s", OutData);
		temp = temp->nxt;
	}
}

static void exportEntry()
{
	ExternOut_t * temp = FirstEntryOut;

	while (temp->nxt != NULL)
	{
		fprintf(OutputFileFD, "%s %04d\n", temp->varName, temp->Address);
		temp = temp->nxt;
	}
}