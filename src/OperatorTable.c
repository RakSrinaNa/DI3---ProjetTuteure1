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
    for(i = OperatorTable_getRecordCount(table) - 1; i >= 0; i--) /* Remove each record */
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
	char tempLineUser[OPERATORTABLE_MAXNAMESIZE + 1]; /* Buffer to read the user */
	char tempLinePassword[OPERATORTABLE_MAXPASSWORDSIZE + 1]; /* Buffer to read the password */
	char * userResult;
	char * passwordResult;
	OperatorTable * operatorTable = OperatorTable_create();
	if((operatorsFile = fopen(filename, "r")) == NULL)
	{
		fatalError("Couldn't open file");
	}
	while((userResult = fgets(tempLineUser, OPERATORTABLE_MAXNAMESIZE + 1, operatorsFile)) != NULL && (passwordResult = fgets(tempLinePassword, OPERATORTABLE_MAXPASSWORDSIZE + 1, operatorsFile)) != NULL) /* While we get a user and password from the file */
	{
		if(icaseEndWith("\n", userResult)) /* If we read a \n, we cut it */
		{
			userResult[stringLength(userResult) - 1] = '\0';
		}
		if(icaseEndWith("\n", passwordResult)) /* If we read a \n, we cut it */
		{
			passwordResult[stringLength(passwordResult) - 1] = '\0';
		}
		decrypt(OperatorCryptKey, userResult); /* Decrypt the user string */
		decrypt(OperatorCryptKey, passwordResult); /* Decrypt the password string */
		OperatorTable_setOperator(operatorTable, userResult, passwordResult); /* Save the operator in the table */
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
	for(indexSave = 0; indexSave < OperatorTable_getRecordCount(table); indexSave++) /* For each user */
	{
		char * encryptedUser = duplicateString(OperatorTable_getName(table, indexSave)); /* Get the username */
        char * encryptedPassword = duplicateString(OperatorTable_getPassword(table, indexSave)); /* Get the password */
		encrypt(OperatorCryptKey, encryptedUser); /* Encrypt user */
		encrypt(OperatorCryptKey, encryptedPassword); /* Encrypt password */
		if(fputs(encryptedUser, operatorsFile) < 0) /* If writing the encrypted user worked */
		{
			fatalError("Error writing in file!");
		}
		if(fputs("\n", operatorsFile) < 0) /* If writing the \n worked */
		{
			fatalError("Error writing in file!");
		}
		if(fputs(encryptedPassword, operatorsFile) < 0) /* If writing the encrypted password worked */
		{
			fatalError("Error writing in file!");
		}
		if(fputs("\n", operatorsFile) < 0) /* If writing the \n worked */
		{
			fatalError("Error writing in file!");
		}
		free(encryptedUser);
		free(encryptedPassword);
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
        fatalError("The record is out of the table");
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
		fatalError("The record is out of the table");
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
	for(operatorIndex = 0; operatorIndex < OperatorTable_getRecordCount(table); operatorIndex++) /* For each user */
	{
		if(icaseCompareString(name, OperatorTable_getName(table, operatorIndex)) == 0) /* If the name is the good one */
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
	int operatorIndex = OperatorTable_findOperator(table, name);
	if(operatorIndex < 0) /* If the user doesn't exist */
	{
		operatorIndex = OperatorTable_getRecordCount(table);
		char *** newRecords = (char ***)realloc(table->records, ((long unsigned int)operatorIndex + 1U) * sizeof(char **)); /* Realloc table to be able to contain a new user */
		if(newRecords == NULL) /* If the realloc failed */
		{
			fatalError("Realloc error!");
		}
		table->records = newRecords;
		if((table->records[operatorIndex] = (char **) malloc(2 * sizeof(char *))) == NULL) /* If the allocation of the username failed */
		{
			fatalError("Malloc error!");
		}
		table->records[operatorIndex][0] = duplicateString(name); /* Set the name */
		table->recordCount++;
	}
	else /* If the user was already present */
	{
		free(table->records[operatorIndex][1]); /* Free the password string */
	}
	table->records[operatorIndex][1] = duplicateString(password); /* Set the password for the user */
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
		fatalError("The record is out of the table");
	}
	free(table->records[recordIndex][0]); /* Free user string */
	free(table->records[recordIndex][1]); /* Free password string */
	free(table->records[recordIndex]); /* Free the array of the user */
	for(i = recordIndex; i < count - 1; i++) /* Shift the users with higher ID to their ID - 1 */
	{
		table->records[i] = table->records[i+1];
	}
	table->recordCount--;
	count--;
	if(count == 0) /* If the table is empty, reset table */
	{
		free(table->records);
		table->records = NULL;
	}
	else
	{
		char *** newRecords = (char ***)realloc(table->records, (long unsigned int)count * sizeof(char **)); /*  Reallocating to a lower size */
		if(newRecords == NULL) /* If the realloc failed */
		{
			fatalError("Realloc error!");
		}
		table->records = newRecords;
	}
}
