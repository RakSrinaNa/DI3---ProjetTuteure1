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
 * $Id: Document.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <Document.h>
#include <DocumentUtil.h>
#include <DocumentRowList.h>
#include <MyString.h>

/** Initialize a document
 * @param document a pointer to a document
 * @warning An initialized document must be finalized by Document_finalize() to free all resources
 */
void IMPLEMENT(Document_init)(Document * document) {
    CustomerRecord_init(&(document->customer));
    document->editDate = duplicateString("");
    document->expiryDate = duplicateString("");
    document->docNumber = duplicateString("");
    document->object = duplicateString("");
    document->operator = duplicateString("");
    document->rows = NULL;
    document->typeDocument = QUOTATION;
}

/** Finalize a document
 * @param document the document to finalize
 * @warning document must have been initialized
 */
void IMPLEMENT(Document_finalize)(Document * document) {
    free(document->editDate);
    free(document->expiryDate);
    free(document->docNumber);
    free(document->object);
    free(document->operator);
    if(document->rows != NULL)
    {
        DocumentRowList_finalize(&(document->rows));
        document->rows = NULL;
    }
}

/** Save the content of a document to a file
 * @param document the document
 * @param filename the file name
 * @warning document must have been initialized
 */
void IMPLEMENT(Document_saveToFile)(Document * document, const char * filename) {
    FILE * file;
    int typeDocumentID = (int) document->typeDocument;
    int rowsCount = DocumentRowList_getRowCount(document->rows);
    DocumentRow * currentRow = document->rows;
    if((file = fopen(filename, "w+b")) == NULL)
    {
        fatalError("Error opening file");
    }
    CustomerRecord_write(&(document->customer), file);
    writeString(document->editDate, file);
    writeString(document->expiryDate, file);
    writeString(document->docNumber, file);
    writeString(document->object, file);
    writeString(document->operator, file);
    if(fwrite(&rowsCount, sizeof(int), 1, file) != 1)
    {
        fatalError("Write error");
    }
    while(currentRow != NULL) /* Write all the rows in file */
    {
        DocumentRow_writeRow(currentRow, file);
        currentRow = currentRow->next;
    }
    if(fwrite(&typeDocumentID, sizeof(int), 1, file) != 1)
    {
        fatalError("Write error");
    }
    fclose(file);
}

/** Load the content of a document from a file
 * @param document the document to fill
 * @param filename the file name
 * @warning document must have been initialized
 */
void IMPLEMENT(Document_loadFromFile)(Document * document, const char * filename) {
    int i;
    FILE * file;
    int typeDocumentID;
    int rowsCount;
    DocumentRow * currentRow;
    if((file = fopen(filename, "rb")) == NULL)
    {
        fatalError("Error opening file");
    }
    Document_finalize(document);
    CustomerRecord_read(&(document->customer), file);
    document->editDate = readString(file);
    document->expiryDate = readString(file);
    document->docNumber = readString(file);
    document->object = readString(file);
    document->operator = readString(file);
    if(fread(&rowsCount, sizeof(int), 1, file) != 1)
    {
        fatalError("Read error");
    }

    for(i = 0; i < rowsCount; i++)
    {
        if(i == 0) /* If it's the first element */
        {
            currentRow = DocumentRow_readRow(file);
            document->rows = currentRow;
        }
        else
        {
            currentRow->next = DocumentRow_readRow(file);
            currentRow = currentRow->next;
        }
    }
    if(fread(&typeDocumentID, sizeof(int), 1, file) != 1)
    {
        fatalError("Read error");
    }
    document->typeDocument = (TypeDocument) typeDocumentID;
    fclose(file);
}
