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
 * $Id: Dictionary.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <Dictionary.h>


/** Create an empty dictionary on the heap
 * @return a new dictionary
 * @warning the dictionary should be destroyed using Dictionary_destroy()
 */
Dictionary * IMPLEMENT(Dictionary_create)(void)
{
    Dictionary * dictionary;
    if((dictionary = (Dictionary *)malloc(sizeof(Dictionary))) == NULL)
    {
        fatalError("Malloc error");
    }
    dictionary->count = 0;
    dictionary->entries = NULL;
    return dictionary;
}

/** Destroy a dictionary
 * @param dictionary the dictionary
 */
void IMPLEMENT(Dictionary_destroy)(Dictionary * dictionary)
{
    int i;
    for(i = 0; i < dictionary->count; i++)
    {
        DictionaryEntry * dictionaryEntry = &(dictionary->entries[i]);
        free(dictionaryEntry->name);
        if(dictionaryEntry->type == STRING_ENTRY)
        {
            free(dictionaryEntry->value.stringValue);
        }
    }
    free(dictionary->entries);
    free(dictionary);
}

/** Get a pointer on the entry associated with the given entry name
 * @param dictionary the dictionary
 * @param name the name of the entry
 * @return a pointer on the entry or NULL if the entry was not found
 */
DictionaryEntry * IMPLEMENT(Dictionary_getEntry)(Dictionary * dictionary, const char * name)
{
    int i;
    for(i = 0; i < dictionary->count; i++)
    {
        DictionaryEntry * dictionaryEntry = &(dictionary->entries[i]);
        if(icaseCompareString(name, dictionaryEntry->name) == 0)
        {
            return dictionaryEntry;
        }
    }
    return NULL;
}

/** Define or change a dictionary entry as a string
 * @param dictionary the dictionary
 * @param name the entry name
 * @param value the value
 */
void IMPLEMENT(Dictionary_setStringEntry)(Dictionary * dictionary, const char * name, const char * value)
{
    DictionaryEntry * dictionaryEntry = Dictionary_getEntry(dictionary, name);
    if(dictionaryEntry == NULL)
    {
        DictionaryEntry * newEntries;
        dictionary->count++;
        if((newEntries = (DictionaryEntry *)realloc(dictionary->entries, (long unsigned int)dictionary->count * sizeof(DictionaryEntry))) == NULL)
        {
            fatalError("Realloc error");
        }
        dictionary->entries = newEntries;
        dictionaryEntry = (dictionary->entries) + dictionary->count - 1;
        dictionaryEntry->name = duplicateString(name);
    }
    else if(dictionaryEntry->type == STRING_ENTRY)
    {
        free(dictionaryEntry->value.stringValue);
    }
    dictionaryEntry->type = STRING_ENTRY;
    dictionaryEntry->value.stringValue = duplicateString(value);
}

/** Define or change a dictionary entry as a number
 * @param dictionary the dictionary
 * @param name the entry name
 * @param value the value
 */
void IMPLEMENT(Dictionary_setNumberEntry)(Dictionary * dictionary, const char * name, double value)
{
    DictionaryEntry * dictionaryEntry = Dictionary_getEntry(dictionary, name);
    if(dictionaryEntry == NULL)
    {
        DictionaryEntry * newEntries;
        dictionary->count++;
        if((newEntries = (DictionaryEntry *)realloc(dictionary->entries, (long unsigned int)dictionary->count * sizeof(DictionaryEntry))) == NULL)
        {
            fatalError("Realloc error");
        }
        dictionary->entries = newEntries;
        dictionaryEntry = (dictionary->entries) + dictionary->count - 1;
        dictionaryEntry->name = duplicateString(name);
    }
    else if(dictionaryEntry->type == STRING_ENTRY)
    {
        free(dictionaryEntry->value.stringValue);
    }
    dictionaryEntry->type = NUMBER_ENTRY;
    dictionaryEntry->value.numberValue = value;
}

/** Create a new string on the heap which is the result of the formatting of format according to the dictionary content
 * @param dictionary the dictionary
 * @param format the string to format
 * @return a new string created on the heap
 * @warning the user is responsible for freeing the returned string
 */
char * IMPLEMENT(Dictionary_format)(Dictionary * dictionary, const char * format)
{
    return provided_Dictionary_format(dictionary, format);
}
