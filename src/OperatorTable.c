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

/**
 * Create an empty table of operators.
 * @return the new table
 * @relates OperatorTable
 */
OperatorTable * IMPLEMENT(OperatorTable_create)(void) {
    return provided_OperatorTable_create();
}

/** Free a table of operators.
 * @param table a pointer to the table to free
 * @relates OperatorTable
 */
void IMPLEMENT(OperatorTable_destroy)(OperatorTable * table) {
    provided_OperatorTable_destroy(table);
}

/** Load a table of operators from a file.
 * @param filename the file name
 * @return the new table
 * @relates OperatorTable
 */
OperatorTable * IMPLEMENT(OperatorTable_loadFromFile)(const char * filename) {
    return provided_OperatorTable_loadFromFile(filename);
}

/** Save a table of operators to a file.
 * @param table the table of operators
 * @param filename the file name
 * @relates OperatorTable
 */
void IMPLEMENT(OperatorTable_saveToFile)(OperatorTable * table, const char * filename) {
    provided_OperatorTable_saveToFile(table, filename);
}

/** Get the number of records of a table of operators
 * @param table the table of operators
 * @return the number of records
 * @relates OperatorTable
 */
int IMPLEMENT(OperatorTable_getRecordCount)(OperatorTable * table) {
    return provided_OperatorTable_getRecordCount(table);
}

/** Get the name of a record of a table of operators.
 * @param table the table of operators
 * @param recordIndex the record number
 * @return the name of the operator
 * @relates OperatorTable
 */
const char * IMPLEMENT(OperatorTable_getName)(OperatorTable * table, int recordIndex) {
    return provided_OperatorTable_getName(table, recordIndex);
}

/** Get the password of a record of a table of operators.
 * @param table the table of operators
 * @param recordIndex the record index
 * @return the paswword of the operator
 * @relates OperatorTable
 */
const char * IMPLEMENT(OperatorTable_getPassword)(OperatorTable * table, int recordIndex) {
    return provided_OperatorTable_getPassword(table, recordIndex);
}

/** Get the record index associated with an operator name.
 * @param table the table of operators
 * @param name the name of the operator
 * @return the index of the operator or -1 if the operator is not in the table
 * @relates OperatorTable
 */
int IMPLEMENT(OperatorTable_findOperator)(OperatorTable * table, const char * name) {
    return provided_OperatorTable_findOperator(table, name);
}

/** Define or change the password of an operator
 * @param table the table of operators
 * @param name the name of the operator
 * @param password the password of the operator
 * @return the index of the operator in the table
 * @relates OperatorTable
 */
int IMPLEMENT(OperatorTable_setOperator)(OperatorTable * table, const char * name, const char * password) {
    return provided_OperatorTable_setOperator(table, name, password);
}

/** Remove an operator from the table.
 * @param table the table of operators
 * @param recordIndex the index of the record to remove
 * @relates OperatorTable
 */
void IMPLEMENT(OperatorTable_removeRecord)(OperatorTable * table, int recordIndex) {
    provided_OperatorTable_removeRecord(table, recordIndex);
}

