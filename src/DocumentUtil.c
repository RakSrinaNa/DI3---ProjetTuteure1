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
 * $Id: DocumentUtil.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <DocumentUtil.h>

static void reverseString(char * string, size_t length);
static void exchangeChar (char * c1, char * c2);
static char getCharNumber(const long int number);

/** Create a new string on the heap which represents the parameter as a number in basis 36.
 * @param id the number to convert
 * @return a new string
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(computeDocumentNumber)(long id) {
    char * converted;
    unsigned int i;
    int base = 36;
    char * tempChar;
    unsigned int charCount = 0;
    if((tempChar = (char *) malloc(7 * sizeof(char))) == NULL)
    {
        fatalError("Erreur allocation malloc!");
    }
    while(id != 0) /* While we still have numbers in base 10 to convert */
    {
        tempChar[charCount] = getCharNumber(id % base); /* Writing number in base 36 reversed */
        charCount++;
        id /= base;
    }
    if((converted = (char *) malloc((charCount + 1) * sizeof(char))) == NULL) /* Allocate only the necessary size */
    {
        fatalError("Erreur allocation malloc!");
    }
    for(i = 0; i < charCount; i++)
    {
        converted[i] = tempChar[i];
    }
    converted[charCount] = '\0';
    free(tempChar);
    reverseString(converted, charCount + 1);
    return converted;
}

static void reverseString(char * string, size_t length)
{
	unsigned int i;
	size_t maxIt = (length - 1U) / 2U;
	for(i = 0; i < maxIt; i++)
	{
		exchangeChar(&string[i], &string[length - 2U - i]);
	}
}

static void exchangeChar (char * c1, char * c2)
{
	char temp = *c1;
	*c1 = *c2;
	*c2 = temp;
}

static char getCharNumber(const long int number)
{
    if(number < 10)
    {
        return (char) ('0' + number);
    }
    return (char) ('A' + number - 10);
}

/** Create a new string on the heap which represents the date in the format DD/MM/YYYY.
 * @param day the day
 * @param month the month
 * @param year the year
 * @return a new string
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * IMPLEMENT(formatDate)(int day, int month, int year) {
    char dateFormat[11];
    sprintf(dateFormat, "%02d/%02d/%04d", day, month, year);
    return duplicateString(dateFormat);
}

/** Write a string in a binary file
 * @param str the string
 * @param file the file
 */
void IMPLEMENT(writeString)(const char * str, FILE * file) {
    size_t strLength = stringLength(str) + 1U;
    if(fwrite(&strLength, sizeof(size_t), 1, file) != 1)
    {
        fatalError("Error write");
    }
    if(fwrite(str, strLength, 1, file) != 1)
    {
        fatalError("Error write");
    }
}

/** Read a string from a binary file
 * @param file the file
 * @return a new string created on the heap which contains the read string
 * @see writeString()
 */
char * IMPLEMENT(readString)(FILE * file) {
    char * str;
    size_t strLength = 0;
    if(fread(&strLength, sizeof(size_t), 1, file) != 1)
    {
        fatalError("Read error");
    }
    if((str = (char *) malloc(strLength * sizeof(char))) == NULL)
    {
        fatalError("Malloc error");
    }
    if(fread(str, strLength, 1, file) != 1)
    {
        fatalError("Read error");
    }
    return str;
}
