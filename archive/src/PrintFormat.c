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
static char * appendLines(char * oldLine, char * newLine);
static char * deleteLastNLineChar(char * line);

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
    int currentCategory = -1;
    if((file = fopen(filename, "r")) == NULL)
    {
        fatalError("Error opening file");
    }

    while(currentCategory != 4 && (lineRead = readLine(file)) != NULL) /* While we didn't reach the end of file or the .END */
    {
        if((icaseStartWith(".NAME", lineRead) == 1) || (icaseStartWith(".HEADER", lineRead) == 1) || (icaseStartWith(".ROW", lineRead) == 1) || (icaseStartWith(".FOOTER", lineRead) == 1) || (icaseStartWith(".END", lineRead) == 1)) /* If we found a marker (.NAME, .HEADER ...) */
        {
            if(icaseStartWith("NAME", lineRead + 1) == 1)
            {
                if(currentCategory != -1)
                {
                    fatalError("File format is not correct");
                }
                currentCategory = 0;
                free(format->name);
                format->name = subString(lineRead + stringLength(".NAME") + 1, lineRead + stringLength(lineRead) - 1); /* Read the name after the ".NAME " */
            }
            else if(icaseStartWith("HEADER", lineRead + 1) == 1)
            {
                if(currentCategory != 0)
                {
                    fatalError("File format is not correct");
                }
                currentCategory = 1;
            }
            else if(icaseStartWith("ROW", lineRead + 1) == 1)
            {
                format->header = deleteLastNLineChar(format->header);
                if(currentCategory != 1)
                {
                    fatalError("File format is not correct");
                }
                currentCategory = 2;
            }
            else if(icaseStartWith("FOOTER", lineRead + 1) == 1)
            {
                format->row = deleteLastNLineChar(format->row);
                if(currentCategory != 2)
                {
                    fatalError("File format is not correct");
                }
                currentCategory = 3;
            }
            else if(icaseStartWith("END", lineRead + 1) == 1)
            {
                format->footer = deleteLastNLineChar(format->footer);
                if(currentCategory != 3)
                {
                    fatalError("File format is not correct");
                }
                currentCategory = 4;
            }
            else
            {
                fatalError("Category not regognized");
            }
        }
        else
        {
            switch(currentCategory)
            {
                case 1:
                    format->header = appendLines(format->header, lineRead);
                break;
                case 2:
                    format->row = appendLines(format->row, lineRead);
                break;
                case 3:
                    format->footer = appendLines(format->footer, lineRead);
                break;
            }
        }
        free(lineRead);
    }
    fclose(file);
}

static char * appendLines(char * oldLine, char * newLine)
{
    char * processedLine = concatenateString(oldLine, newLine);
    free(oldLine);
    return processedLine;
}

static char * deleteLastNLineChar(char * line)
{
    size_t lineLength = stringLength(line);
    if(line[lineLength - 1] == '\n')
    {
        char * lineToFree = line;
        line = subString(line, line + lineLength - 1);
        free(lineToFree);
    }
    return line;
}

static char * readLine(FILE * fichier)
{
    char buffer[1024] = {0};
    char * stringRead = duplicateString("");
    char * partRead;
    do /* While we didn't reach the end of the line */
    {
        if((partRead = fgets(buffer, 1024, fichier)) == NULL)
        {
            return stringRead;
        }
        stringRead = appendLines(stringRead, partRead);
    } while(buffer[stringLength(buffer) - 1] != '\n');
    return stringRead;
}
