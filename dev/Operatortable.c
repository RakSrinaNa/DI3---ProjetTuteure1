#include <stdlib.h>
#include <printf.h>
#include "Operatortable.h"
#include "MyString.h
#include "MyString.h"

void fatalError(char * str)
{
	printf("%s\n", str);
	exit(1);
}

OperatorTable * OperatorTable_create(void)
{
	OperatorTable * operatorTable = (OperatorTable *) malloc(sizeof(OperatorTable));
	if(operatorTable == NULL)
	{
		fatalError("Erreur allocation malloc!");
	}
	operatorTable->records = NULL;
	return operatorTable;
}

int OperatorTable_getRecordCount(OperatorTable * table)
{
	return table->recordCount;
}

const char * OperatorTable_getName(OperatorTable * table, int recordIndex)
{
	if(recordIndex >= OperatorTable_getRecordCount(table))
	{
		fatalError("L'enregistrement sort de la table!");
	}
	return table->records[recordIndex][0];
}

const char * OperatorTable_getPassword(OperatorTable * table, int recordIndex)
{
	if(recordIndex > OperatorTable_getRecordCount(table))
	{
		fatalError("L'enregistrement sort de la table!");
	}
	return table->records[recordIndex][1];
}

int OperatorTable_findOperator(OperatorTable * table, const char * name)
{
	int operatorIndex;
	for(operatorIndex = 0; operatorIndex < OperatorTable_getRecordCount(table); operatorIndex++)
	{
		if(icaseCompareString(name, OperatorTable_getName(table, operatorIndex)) == 0)
		{
			return operatorIndex;
		}
	}
	return -1;
}

int OperatorTable_setOperator(OperatorTable * table, const char * name, const char *password)
{
	char *** newRecords;
	int operatorIndex = OperatorTable_findOperator(table, name);
	if(operatorIndex < 0)
	{
		operatorIndex = OperatorTable_getRecordCount(table);
		newRecords = (char ***)realloc(table->records, ((long unsigned int)operatorIndex + 1U) * sizeof(char **));
		if(newRecords == NULL)
		{
			fatalError("Erreur realloc!");
		}
		table->records = newRecords;
		if((table->records[operatorIndex] = (char **) malloc(2 * sizeof(char *))) == NULL)
		{
			fatalError("Erreur malloc!");
		}
		table->records[operatorIndex][0] = duplicateString(name);
		table->recordCount++;
	}
	else
	{
		free(table->records[operatorIndex][1]);
	}
	table->records[operatorIndex][1] = duplicateString(password);
	return operatorIndex;
}

void OperatorTable_removeRecord(OperatorTable * table, int recordIndex)
{
	int i;
	int count = OperatorTable_getRecordCount(table);
	if(recordIndex >= count)
	{
		fatalError("Enregistrement hors de la table");
	}
	free(table->records[recordIndex][0]);
	free(table->records[recordIndex][1]);
	free(table->records[recordIndex]);
	for(i = recordIndex; i < count; i++)
	{
		table->records[i] = table->records[i+1];
	}
	table->recordCount--;
	count--;
	if(count == 0)
	{
		free(table->records);
		table->records = NULL;
	}
	else
	{
		char *** newRecords = (char ***)realloc(table->records, (long unsigned int)count * sizeof(char **));
		if(newRecords == NULL)
		{
			fatalError("Erreur realloc!");
		}
		table->records = newRecords;
	}
}

void OperatorTable_destroy(OperatorTable * table)
{
	int i;
	for(i = OperatorTable_getRecordCount(table) - 1; i >= 0; i--)
	{
		OperatorTable_removeRecord(table, i);
	}
	free(table->records);
	free(table);
}
