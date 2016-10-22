#ifndef PROJETTUTORE1_OPERATORTABLE_H
#define PROJETTUTORE1_OPERATORTABLE_H

/** The maximal length in characters of the name of an operator */
#define OPERATORTABLE_MAXNAMESIZE 20UL
/** The maximal length in characters of the password of an operator */
#define OPERATORTABLE_MAXPASSWORDSIZE 20UL

/** The dynamic table of operators */
typedef struct {
	/** The number of operators in the table */
	int recordCount;
	/** The data about the operators. It's a 2D array of strings.
	 * Note: records[operatorId][0] is the name of the operatorId'th operator
	 * Note: records[operatorId][1] is the password of the operatorId'th operator
	 */
	char *** records;
} OperatorTable;

OperatorTable * OperatorTable_create(void);
int OperatorTable_getRecordCount(OperatorTable * table);
const char * OperatorTable_getName(OperatorTable * table, int recordIndex);
const char * OperatorTable_getPassword(OperatorTable * table, int recordIndex);
int OperatorTable_findOperator(OperatorTable * table, const char * name);
int OperatorTable_setOperator(OperatorTable * table, const char * name, const char *password );
void OperatorTable_removeRecord(OperatorTable * table, int recordIndex);
void OperatorTable_destroy(OperatorTable * table);

#endif
