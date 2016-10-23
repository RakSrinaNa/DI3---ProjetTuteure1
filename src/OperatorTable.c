/*
 * Copyright 2010 Sébastien Aupetit <sebastien.aupetit@univ-tours.fr>
 *
 * This source code is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this source code. If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id: OperatorTable.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <OperatorTable.h>
#include <EncryptDecrypt.h>
#include "MyString.h"

/**
 * Create an empty table of operators.
 * @return the new table
 * @relates OperatorTable
 */
OperatorTable * IMPLEMENT(OperatorTable_create)(void) {
    OperatorTable * operatorTable = (OperatorTable *) malloc(sizeof(OperatorTable));
    if(operatorTable == NULL)
    {
        fatalError("Erreur allocation malloc!");
    }
    operatorTable->recordCount = 0;
    operatorTable->records = NULL;
    return operatorTable;
}

/** Free a table of operators.
 * @param table a pointer to the table to free
 * @relates OperatorTable
 */
void IMPLEMENT(OperatorTable_destroy)(OperatorTable * table) {
    int i;
    for(i = OperatorTable_getRecordCount(table) - 1; i >= 0; i--)
    {
        OperatorTable_removeRecord(table, i);
    }
    free(table->records);
    free(table);
}

/** Load a table of operators from a file.
 * @param filename the file name
 * @return the new table
 * @relates OperatorTable
 */
OperatorTable * IMPLEMENT(OperatorTable_loadFromFile)(const char * filename) {
	FILE * operatorsFile;
	char tempLineUser[OPERATORTABLE_MAXNAMESIZE + 1];
	char tempLinePassword[OPERATORTABLE_MAXPASSWORDSIZE + 1];
	char * userResult;
	char * passwordResult;
	OperatorTable * operatorTable = OperatorTable_create();
	if((operatorsFile = fopen(filename, "r")) == NULL)
	{
		fatalError("Couldn't open file");
	}
	while((userResult = fgets(tempLineUser, OPERATORTABLE_MAXNAMESIZE, operatorsFile)) != NULL && (passwordResult = fgets(tempLinePassword, OPERATORTABLE_MAXPASSWORDSIZE, operatorsFile)) != NULL)
	{
		if(icaseEndWith("\n", userResult))
		{
			userResult[stringLength(userResult) - 1] = '\0';
		}
		if(icaseEndWith("\n", passwordResult))
		{
			passwordResult[stringLength(passwordResult) - 1] = '\0';
		}
		OperatorTable_setOperator(operatorTable, userResult, passwordResult);
	}
	if(fclose(operatorsFile) != 0)
	{
		fatalError("Couldn't close file");
	}
	return operatorTable;
}

/** Save a table of operators to a file.
 * @param table the table of operators
 * @param filename the file name
 * @relates OperatorTable
 */
void IMPLEMENT(OperatorTable_saveToFile)(OperatorTable * table, const char * filename) {
	FILE * operatorsFile;
	int indexSave;
	if((operatorsFile = fopen(filename, "w")) == NULL)
	{
		fatalError("Couldn't open file");
	}
	for(indexSave = 0; indexSave < OperatorTable_getRecordCount(table); indexSave++)
	{
		if(fputs(OperatorTable_getName(table, indexSave), operatorsFile) < 0)
		{
			fatalError("Error writing in file!");
		}
		if(fputs("\n", operatorsFile) < 0)
		{
			fatalError("Error writing in file!");
		}
		if(fputs(OperatorTable_getPassword(table, indexSave), operatorsFile) < 0)
		{
			fatalError("Error writing in file!");
		}
		if(fputs("\n", operatorsFile) < 0)
		{
			fatalError("Error writing in file!");
		}
	}
	if(fclose(operatorsFile) != 0)
	{
		fatalError("Couldn't close file");
	}
}

/** Get the number of records of a table of operators
 * @param table the table of operators
 * @return the number of records
 * @relates OperatorTable
 */
int IMPLEMENT(OperatorTable_getRecordCount)(OperatorTable * table) {
    return table->recordCount;
}

/** Get the name of a record of a table of operators.
 * @param table the table of operators
 * @param recordIndex the record number
 * @return the name of the operator
 * @relates OperatorTable
 */
const char * IMPLEMENT(OperatorTable_getName)(OperatorTable * table, int recordIndex) {
    if(recordIndex < 0 && recordIndex >= OperatorTable_getRecordCount(table))
    {
        fatalError("L'enregistrement sort de la table!");
    }
    return table->records[recordIndex][0];
}

/** Get the password of a record of a table of operators.
 * @param table the table of operators
 * @param recordIndex the record index
 * @return the paswword of the operator
 * @relates OperatorTable
 */
const char * IMPLEMENT(OperatorTable_getPassword)(OperatorTable * table, int recordIndex) {
	if(recordIndex < 0 && recordIndex > OperatorTable_getRecordCount(table))
	{
		fatalError("L'enregistrement sort de la table!");
	}
	return table->records[recordIndex][1];
}

/** Get the record index associated with an operator name.
 * @param table the table of operators
 * @param name the name of the operator
 * @return the index of the operator or -1 if the operator is not in the table
 * @relates OperatorTable
 */
int IMPLEMENT(OperatorTable_findOperator)(OperatorTable * table, const char * name) {
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

/** Define or change the password of an operator
 * @param table the table of operators
 * @param name the name of the operator
 * @param password the password of the operator
 * @return the index of the operator in the table
 * @relates OperatorTable
 */
int IMPLEMENT(OperatorTable_setOperator)(OperatorTable * table, const char * name, const char * password) {
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

/** Remove an operator from the table.
 * @param table the table of operators
 * @param recordIndex the index of the record to remove
 * @relates OperatorTable
 */
void IMPLEMENT(OperatorTable_removeRecord)(OperatorTable * table, int recordIndex) {
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
