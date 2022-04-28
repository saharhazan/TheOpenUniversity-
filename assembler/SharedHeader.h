#ifndef SHARED_HEADER_H_
#define SHARED_HEADER_H_
/* Inclusion */
#include "Configurations.h"

/* User-defined data types */
enum Instruction_e
{
	ADD = 0,
	SUB,
	AND,
	OR,
	NOR,
	MOVE,
	MVHI,
	MVLO,
	ADDI,
	SUBI,
	ANDI,
	ORI,
	NORI,
	BNE,
	BEQ,
	BLT,
	BGT,
	LB,
	SB,
	LW,
	SW,
	LH,
	SH,
	JMP,
	LA,
	CALL,
	STOP
};
typedef enum Instruction_e Instruction_e;

enum MemAllocTypes_e
{
	DB = 0,
	DH,
	DW,
	ASCIZ,
	ENTRY,
	EXTERN
};
typedef enum MemAllocTypes_e MemAllocTypes_e;

struct AllocatedSpace_t
{
	signed int Address;
	signed char count;
	char label[LABEL_SIZE];
	char code[LINE_SIZE];
	signed long * Data;
	MemAllocTypes_e InstructionType;
	struct AllocatedSpace_t * nxtMemoryAlloc;
};
typedef struct AllocatedSpace_t AllocatedSpace_t;


enum Instruction_Type_t {
	R_TYPE = 0,
	I_TYPE,
	J_TYPE
};
typedef enum Instruction_Type_t Instruction_Type_t;

struct Instruction_t
{
	signed int immidiateValue;
	unsigned char RS;
	unsigned char RD;
	unsigned char RT;
	unsigned char function;
	unsigned char opCode;
	Instruction_e Instruction_Name;
	Instruction_Type_t type;
	signed long Address;
	int lineNumber;
	unsigned int Reg : 1;
};
typedef struct Instruction_t Instruction_t;

struct CodeMetaData_t
{
	signed int Address;
	signed long Machine_Code;
	char code[80];
	char label[LABEL_SIZE];
	struct CodeMetaData_t * nxtCodeLine;
	Instruction_t instruction_information;
};
typedef struct CodeMetaData_t CodeMetaData_t;

struct ExternVariable_t
{
	char varName[MAX_VAR_NAME];
	char label[LABEL_SIZE];
	char code[LINE_SIZE];
	struct ExternVariable_t * nxtVar;
};
typedef struct  ExternVariable_t ExternVariable_t;

struct ExternOut_t
{
	char varName[MAX_VAR_NAME];
	signed int Address;

	struct ExternOut_t * nxt;
};
typedef struct ExternOut_t ExternOut_t;

#endif
