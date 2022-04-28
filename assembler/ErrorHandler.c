/* Inclusion */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ErrorHandler.h"
#include "Configurations.h"

/* Private variables */
Error_t * error_list_head = NULL;

static char * error_messages[][50] = {{
"Unrecognized assembly instruction\0",
"Unrecognized register number\0",
"Immidiate value MUST be integer\0",
"Syntax error\0",
"Unrecognized label\0"
}};

/* private functions */
static Error_t* create_node(ErrorType_e errorType, int lineNumber);
static void assign_node(Error_t* node_to_asssign, Error_t* pre_node, Error_t* nxt_node);


int number_of_errors = 0;
extern char FullFileName[FILE_NAME_SIZE];

void ErrorHandler_Init()
{
	error_list_head = create_node(0,-1); /*dummy node*/
}

void ErrorHandler_RegisterError(ErrorType_e errorType, int lineNumber)
{
	Error_t * error_to_insert;
	Error_t * trav = error_list_head;

	lineNumber = (lineNumber + 1) / 2;

	number_of_errors++; /* increase number of errors */

	if (ErrorHandler_GetErrorsCount() == 1)
	{
		/* if there is only one error, assign*/
		error_list_head->line_number = lineNumber;
		error_list_head->error_type = errorType;
		error_list_head->error_message = error_messages[0][errorType];
	}
	else
	{
		/* handle more than one error (add it with a certain order) */
		error_to_insert = create_node(errorType, lineNumber);

		/* Put it in order */
		if (error_to_insert->line_number < error_list_head->line_number)
		{
			assign_node(error_to_insert,NULL,error_list_head);
		}
		else
		{
			while (trav->nxt_error != NULL && error_to_insert->line_number > trav->line_number)
			{
				trav = trav->nxt_error;
			}

			if (error_to_insert->line_number >= trav->line_number)
			{
				assign_node(error_to_insert,trav,NULL);
			}
			else
			{
				assign_node(error_to_insert,trav->pre_error,trav);
			}
		}
	}

}

void ErrorHandler_DisplayErrorMessages()
{
	Error_t * temp = error_list_head;
	int count = 1;
	
	printf("%d Errors found in %s\n", ErrorHandler_GetErrorsCount(), FullFileName);
	while (temp != NULL)
	{
		/* while temp (next error) exist, print them.*/
		printf("%-4d%s in line: %d\n", count, temp->error_message, temp->line_number);

		temp = temp->nxt_error;
		count++;
	}
}

int ErrorHandler_GetErrorsCount()
{
	return number_of_errors;
}

static Error_t* create_node(ErrorType_e errorType, int lineNumber)
{
	Error_t *ret_node = (Error_t *)malloc(sizeof(Error_t));
	ret_node->line_number = lineNumber;
	ret_node->error_type = errorType;
	ret_node->error_message = error_messages[0][errorType];
	ret_node->nxt_error = NULL;
	ret_node->pre_error = NULL;

	return ret_node;
}

static void assign_node(Error_t* node_to_asssign, Error_t* pre_node, Error_t* nxt_node)
{
	if(node_to_asssign == NULL)
	{
		fprintf( stderr, "assign_node: null node_to_assign was given.\n");
		return; /*shouldnt get here*/
	}
	if(pre_node != NULL)
	{
		pre_node->nxt_error = node_to_asssign;
		node_to_asssign->pre_error = pre_node;
	}
	if(nxt_node != NULL)
	{
		nxt_node->pre_error = node_to_asssign;
		node_to_asssign->nxt_error = nxt_node;
		if(pre_node == NULL && nxt_node == error_list_head)
		{
			error_list_head = node_to_asssign;	
		}
	}
}