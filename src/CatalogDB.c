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
 * $Id: CatalogDB.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <CatalogDB.h>
#include <CatalogRecord.h>
#include <CatalogRecordEditor.h>

/** The catalog file name */
const char * CATALOGDB_FILENAME = BASEPATH "/data/Catalog.db";

/** Create a database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 */
CatalogDB * IMPLEMENT(CatalogDB_create)(const char * filename) {
    FILE * recordsFile = fopen(filename, "w+b"); /* Open binary file in read/write mode, resetting file if already existing */
	CatalogDB * catalogDB;
	if(recordsFile == NULL)
	{
		return NULL;
	}
	if((catalogDB = (CatalogDB *)malloc(sizeof(CatalogDB))) == NULL) /* Allocate memory for the structure */
	{
        fatalError("Error malloc");
	}
	catalogDB->file = recordsFile;
	catalogDB->recordCount = 0;
    return catalogDB;
}

/** Open an existing database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 */
CatalogDB * IMPLEMENT(CatalogDB_open)(const char * filename) {
    FILE * recordsFile = fopen(filename, "r+b"); /* Open binary file in read/write mode */
	CatalogDB * catalogDB;
	if(recordsFile == NULL)
	{
		return NULL;
	}
	if((catalogDB = (CatalogDB *)malloc(sizeof(CatalogDB))) == NULL) /* Allocate memory for the structure */
	{
		fatalError("Error malloc");
	}
	catalogDB->file = recordsFile;
    fread(&(catalogDB->recordCount), sizeof(int), 1, recordsFile);
	return catalogDB;
}

/** Open if exists or create otherwise a database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 */
CatalogDB * IMPLEMENT(CatalogDB_openOrCreate)(const char * filename) {
    CatalogDB * catalogDB;
	if((catalogDB = CatalogDB_open(filename)) == NULL && (catalogDB = CatalogDB_create(filename)) == NULL) /* Try to load from an existing file, if failed, create it */
	{
		return NULL;
	}
	return catalogDB;
}

/** Flush cached data, close a database and free the structure representing the opened database
 * @param catalogDB a pointer on a CatalogDB representing the opened database
 */
void IMPLEMENT(CatalogDB_close)(CatalogDB * catalogDB) {
    fseek(catalogDB->file, 0, SEEK_SET); /* Go at beginning of file */
	fwrite(&(catalogDB->recordCount), sizeof(int), 1, catalogDB->file); /* Write the number of records */
	fclose(catalogDB->file);
	free(catalogDB);
}

/** Get the number of records of the database
 * @param catalogDB the database
 * @return the number of records
 */
int IMPLEMENT(CatalogDB_getRecordCount)(CatalogDB * catalogDB) {
    return catalogDB->recordCount;
}

/** Create a new string on the heap containing the value of the specified field for the specified record of a database
 * @param catalogDB the database
 * @param recordIndex the record index
 * @param field the field to query
 * @return a new string
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * CatalogDB_getFieldValueAsString(CatalogDB * catalogDB, int recordIndex, int field) {
    char * content = NULL;
    if (catalogDB != NULL) {
        CatalogRecord_FieldProperties properties = CatalogRecord_getFieldProperties(field);
        CatalogRecord record;
        CatalogRecord_init(&record);
        CatalogDB_readRecord(catalogDB, recordIndex, &record);
        content = (*properties.getValue)(&record);
        CatalogRecord_finalize(&record);
    }
    return content;
}

/** Append the specified record at the end of the database
 * @param catalogDB the database
 * @param record the record
 */
void IMPLEMENT(CatalogDB_appendRecord)(CatalogDB * catalogDB, CatalogRecord *record) {
    CatalogDB_writeRecord(catalogDB, CatalogDB_getRecordCount(catalogDB), record);
}

/** Insert the specified record at the given position in the database
 * @param catalogDB the database
 * @param recordIndex the insertion position
 * @param record the record
 */
void IMPLEMENT(CatalogDB_insertRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record) {
    CatalogRecord catalogRecord;
    CatalogRecord_init(&catalogRecord);
    int i;
    for(i = CatalogDB_getRecordCount(catalogDB) - 1; i >= recordIndex; i--) /* Shift evry record starting at out index */
    {
        CatalogDB_readRecord(catalogDB, i, &catalogRecord);
        CatalogDB_writeRecord(catalogDB, i + 1, &catalogRecord);
    }
    CatalogRecord_finalize(&catalogRecord);
    CatalogDB_writeRecord(catalogDB, recordIndex, record); /* Write record */
}

/** Remove a record at a given position from the database
 * @param catalogDB the database
 * @param recordIndex the removal position
 */
void IMPLEMENT(CatalogDB_removeRecord)(CatalogDB * catalogDB, int recordIndex) {
    CatalogRecord catalogRecord;
    CatalogRecord_init(&catalogRecord);
    int i;
    for(i = recordIndex + 1; i < CatalogDB_getRecordCount(catalogDB); i++) /* Shift evry record starting at out index */
    {
        CatalogDB_readRecord(catalogDB, i, &catalogRecord);
        CatalogDB_writeRecord(catalogDB, i - 1, &catalogRecord);
    }
    CatalogRecord_finalize(&catalogRecord);
    catalogDB->recordCount--; /* Reduce the number of records */
}

/** Read a record from the database
 * @param catalogDB the database
 * @param recordIndex the position of the record to read
 * @param record the record to fill with data
 */
void IMPLEMENT(CatalogDB_readRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record) {
    if(recordIndex >= CatalogDB_getRecordCount(catalogDB))
	{
		fatalError("Record index isn't present");
	}
	fseek(catalogDB->file, (long) (sizeof(int) + (long unsigned int)recordIndex * CATALOGRECORD_SIZE), SEEK_SET); /* Seek the position to read */
	CatalogRecord_read(record, catalogDB->file);
}

/** Write a record from the database
 * @param catalogDB the database
 * @param recordIndex the position of the record to write
 * @param record the record containing the data
 */
void IMPLEMENT(CatalogDB_writeRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record) {
    if(recordIndex >= CatalogDB_getRecordCount(catalogDB)) /* If it's a new record */
    {
        catalogDB->recordCount++;
    }
	fseek(catalogDB->file, (long)(sizeof(int) + (long unsigned int)recordIndex * CATALOGRECORD_SIZE), SEEK_SET); /* seek the positio to write */
	CatalogRecord_write(record, catalogDB->file);
}
