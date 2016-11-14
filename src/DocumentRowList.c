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
 * $Id: DocumentRowList.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <DocumentRowList.h>
#include <DocumentUtil.h>

/** Initialize a row
 * @param row the row
 * @warning an initialized row must be finalized by DocumentRow_finalize() to free all resources
 */
void IMPLEMENT(DocumentRow_init)(DocumentRow * row) {
    row->code = duplicateString("");
    row->designation = duplicateString("");
    row->quantity = 0;
    row->unity = duplicateString("");
    row->basePrice = 0;
    row->sellingPrice = 0;
    row->discount = 0;
    row->rateOfVAT = 0;
    row->next = NULL;
}

/** Finalize a row
 * @param row the row
 * @warning document must have been initialized
 */
void IMPLEMENT(DocumentRow_finalize)(DocumentRow * row) {
    free(row->code);
    free(row->designation);
    free(row->unity);
}

/** Create a new row on the heap and initialize it
 * @return the new row
 */
DocumentRow * IMPLEMENT(DocumentRow_create)(void) {
    DocumentRow * documentRow;
    if((documentRow = (DocumentRow *) malloc(sizeof(DocumentRow))) == NULL)
    {
        fatalError("Malloc error");
    }
    DocumentRow_init(documentRow);
    return documentRow;
}

/** Finalize and destroy a row previously created on the heap
 * @param row the row
 */
void IMPLEMENT(DocumentRow_destroy)(DocumentRow * row) {
    DocumentRow_finalize(row);
    free(row);
}

/** Initialize a list of rows
 * @param list the address of the pointer on the first cell of the list
 */
void IMPLEMENT(DocumentRowList_init)(DocumentRow ** list) {
    *list = NULL;
}

/** Finalize a list of rows
 * @param list the address of the pointer on the first cell of the list
 * @note Each row of the list are destroyer using DocumentRow_destroy()
 */
void IMPLEMENT(DocumentRowList_finalize)(DocumentRow ** list) {
    while(*list != NULL)
    {
        DocumentRow * nextElement = (*list)->next;
        free(*list);
        *list = nextElement;
    }
}

/** Get a pointer on the rowIndex-th row of the list
 * @param list the pointer on the first cell of the list
 * @param rowIndex the index of the requested row
 * @return a pointer on the rowIndex-th row of the list or NULL if the list contains less rows than the requested one
 */
DocumentRow * IMPLEMENT(DocumentRowList_get)(DocumentRow * list, int rowIndex) {
    int i;
    for(i = 0; i < rowIndex; i++)
    {
        if(list == NULL)
        {
            return NULL;
        }
        list = list->next;
    }
    return list;
}

/**
 * Get the number of rows in the list
 * @param list the pointer on the first cell of the list
 */
int IMPLEMENT(DocumentRowList_getRowCount)(DocumentRow * list) {
    int count = 0;
    while(list != NULL)
    {
        count++;
        list = list->next;
    }
    return count;
}

/** Add a row at the end of the list
 * @param list the address of the pointer on the first cell of the list
 * @param row the row to add
 */
void IMPLEMENT(DocumentRowList_pushBack)(DocumentRow ** list, DocumentRow * row) {
    if(*list == NULL)
    {
        *list = row;
    }
    else
    {
        DocumentRowList_insertAfter(list, DocumentRowList_get(*list, DocumentRowList_getRowCount(*list) - 1), row);
    }
}

/** Insert a row before a given row
 * @param list the address of the pointer on the first cell of the list
 * @param position a pointer on the positioning row
 * @param row the row to insert
 */
void IMPLEMENT(DocumentRowList_insertBefore)(DocumentRow ** list, DocumentRow * position, DocumentRow * row) {
    DocumentRow * beforeElement = NULL;
    DocumentRow * currentElement = *list;
    while(currentElement != NULL && currentElement != position)
    {
        beforeElement = currentElement;
        currentElement = currentElement->next;
    }
    if(currentElement == NULL)
    {
        fatalError("The position isn't in the list");
    }
    if(beforeElement == NULL)
    {
        row->next = *list;
        *list = row;
    }
    else
    {
        row->next = beforeElement->next;
        beforeElement->next = row;
    }
}

/** Insert a row after a given row
 * @param list the address of the pointer on the first cell of the list
 * @param position a pointer on the positioning row
 * @param row the row to insert
 */
void IMPLEMENT(DocumentRowList_insertAfter)(DocumentRow ** list, DocumentRow * position, DocumentRow * row) {
    DocumentRow * currentElement = *list;
    while(currentElement != NULL && currentElement != position)
    {
        currentElement = currentElement->next;
    }
    if(currentElement == NULL)
    {
        fatalError("The position isn't in the list");
    }
    row->next = currentElement->next;
    currentElement->next = row;
}

/** Remove a row from the list
 * @param list the address of the pointer on the first cell of the list
 * @param position the row to remove
 */
void IMPLEMENT(DocumentRowList_removeRow)(DocumentRow ** list, DocumentRow * position) {
    DocumentRow * beforeElement = NULL;
    DocumentRow * currentElement = *list;
    while(currentElement != NULL && currentElement != position)
    {
        beforeElement = currentElement;
        currentElement = currentElement->next;
    }
    if(currentElement == NULL)
    {
        fatalError("The position isn't in the list");
    }
    if(beforeElement == NULL)
    {
        *list = currentElement->next;
    }
    else
    {
        beforeElement->next = currentElement->next;
    }
}

/** Write a row in a binary file
 * @param row the row
 * @param file the opened file
 */
void IMPLEMENT(DocumentRow_writeRow)(DocumentRow * row, FILE * file) {
    if(1) /* TODO */
    {
        provided_DocumentRow_writeRow(row, file);
        return;
    }
    if(fwrite(row->code, stringLength(row->code) + 1U, 1, file) != 1)
    {
        fatalError("Error write");
    }
    if(fwrite(row->designation, stringLength(row->designation) + 1U, 1, file) != 1)
    {
        fatalError("Error write");
    }
    if(fwrite(&(row->quantity), sizeof(double), 1, file) != 1)
    {
        fatalError("Error write");
    }
    if(fwrite(row->unity, stringLength(row->unity) + 1U, 1, file) != 1)
    {
        fatalError("Error write");
    }
    if(fwrite(&(row->basePrice), sizeof(double), 1, file) != 1)
    {
        fatalError("Error write");
    }
    if(fwrite(&(row->sellingPrice), sizeof(double), 1, file) != 1)
    {
        fatalError("Error write");
    }
    if(fwrite(&(row->discount), sizeof(double), 1, file) != 1)
    {
        fatalError("Error write");
    }
    if(fwrite(&(row->rateOfVAT), sizeof(double), 1, file) != 1)
    {
        fatalError("Error write");
    }
}

/** Read a row from a file
 * @param file the opened file
 * @return a new row created on the heap filled with the data
 */
DocumentRow * IMPLEMENT(DocumentRow_readRow)(FILE * file) {
    if(1) /* TODO */
        return provided_DocumentRow_readRow(file);
    DocumentRow * row = DocumentRow_create();
    if(fscanf(file, "%s", row->code) != 1)
    {
        fatalError("Error read");
    }
    if(fscanf(file, "%s", row->designation) != 1)
    {
        fatalError("Error read");
    }
    if(fread(&(row->quantity), sizeof(double), 1, file) != 1)
    {
        fatalError("Error read");
    }
    if(fscanf(file, "%s", row->unity) != 1)
    {
        fatalError("Error read");
    }
    if(fread(&(row->basePrice), sizeof(double), 1, file) != 1)
    {
        fatalError("Error read");
    }
    if(fread(&(row->sellingPrice), sizeof(double), 1, file) != 1)
    {
        fatalError("Error read");
    }
    if(fread(&(row->discount), sizeof(double), 1, file) != 1)
    {
        fatalError("Error read");
    }
    if(fread(&(row->rateOfVAT), sizeof(double), 1, file) != 1)
    {
        fatalError("Error read");
    }
    return row;
}
