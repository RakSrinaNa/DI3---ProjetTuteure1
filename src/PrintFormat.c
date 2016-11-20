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
 * $Id: PrintFormat.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <PrintFormat.h>
#include <Dictionary.h>

static char * readLine(FILE * fichier);

/** Initialize a print format
 * @param format a print format
 * @warning initialized print format should be finalized with PrintFormat_finalize()
 */
void IMPLEMENT(PrintFormat_init)(PrintFormat * format)
{
    format->name = duplicateString("");
    format->header = duplicateString("");
    format->row = duplicateString("");
    format->footer = duplicateString("");
}

/** Finalize a print format
 * @param format the print format
 */
void IMPLEMENT(PrintFormat_finalize)(PrintFormat * format)
{
    free(format->name);
    free(format->header);
    free(format->row);
    free(format->footer);
}

/** Load a print format from a file
 * @param format an initialized print format to fill
 * @param filename the file name
 */
void IMPLEMENT(PrintFormat_loadFromFile)(PrintFormat * format, const char * filename)
{
    FILE * file;
    char * lineRead;
    int currentCategory = 0;
    if((file = fopen(filename, "r")) == NULL)
    {
        fatalError("Error opening file");
    }
    while(currentCategory != 4 && (lineRead = readLine(file)) != NULL)
    {
        if(icaseStartWith(".", lineRead))
        {
            if(icaseStartWith("NAME", lineRead + 1))
            {
                currentCategory = 0;
                free(format->name);
                format->name = duplicateString(lineRead + 6);
            }
            else if(icaseStartWith("HEADER", lineRead + 1))
            {
                currentCategory = 1;
            }
            else if(icaseStartWith("ROW", lineRead + 1))
            {
                currentCategory = 2;
            }
            else if(icaseStartWith("FOOTER", lineRead + 1))
            {
                currentCategory = 3;
            }
            else if(icaseStartWith("END", lineRead + 1))
            {
                currentCategory = 4;
            }
            else
            {
                fatalError("Category not regognized");
            }
        }
        else
        {
            char * newCategory;
            switch(currentCategory)
            {
                case 1:
                    if(compareString(format->header, "") != 0)
                    {
                        newCategory = concatenateString(format->header, "\n");
                        free(format->header);
                        format->header = newCategory;
                    }
                    newCategory = concatenateString(format->header, lineRead);
                    free(format->header);
                    format->header = newCategory;
                break;
                case 2:
                    if(compareString(format->row, "") != 0)
                    {
                        newCategory = concatenateString(format->row, "\n");
                        free(format->row);
                        format->row = newCategory;
                    }
                    newCategory = concatenateString(format->row, lineRead);
                    free(format->row);
                    format->row = newCategory;
                break;
                case 3:
                    if(compareString(format->footer, "") != 0)
                    {
                        newCategory = concatenateString(format->footer, "\n");
                        free(format->footer);
                        format->footer = newCategory;
                    }
                    newCategory = concatenateString(format->footer, lineRead);
                    free(format->footer);
                    format->footer = newCategory;
                break;
            }
        }
        free(lineRead);
    }
    fclose(file);
    if(0)
        provided_PrintFormat_loadFromFile(format, filename);
}

static char * readLine(FILE * fichier)
{
    unsigned int stringReadLength = 1;
    char tempChar[2] = {'\0'};
    char * stringRead = NULL;
    char * charRead;
    while((charRead = fgets(tempChar, 2, fichier)) != NULL && charRead[0] != '\n' && charRead[0] != '\0')
    {
        char * newStringRead = NULL;
        stringReadLength++;
        if((newStringRead = (char *)realloc(stringRead, stringReadLength * sizeof(char))) == NULL)
        {
            fatalError("Error realloc");
        }
        stringRead = newStringRead;
        stringRead[stringReadLength - 2] = *charRead;
    }
    if(stringRead == NULL)
    {
        return NULL;
    }
    stringRead[stringReadLength - 1] = '\0';
    return stringRead;
}
