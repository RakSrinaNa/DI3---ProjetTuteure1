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
 * $Id: EncryptDecrypt.c 247 2010-09-10 10:23:07Z sebtic $
 */

#include <EncryptDecrypt.h>

static char encode(char key, char decoded);
static char decode(char key, char encoded);
static int mod(const int a, const int b);

void IMPLEMENT(encrypt)(const char * key, char * str) {
    size_t keyLength = stringLength(key);
    unsigned int i = 0;
    while(str[i] != '\0') /* While we didn't reached the end */
    {
        char c = str[i];
        if((c >= 'A' && c <= 'Z') || (c >='a' && c <= 'z')) /* If were're in [A-Z]|[a-z] */
        {
            str[i] = encode(toUpperChar(key[i % keyLength]), c);
        }
        i++;
    }
}

void IMPLEMENT(decrypt)(const char * key, char * str) {
    size_t keyLength = stringLength(key);
    unsigned int i = 0;
    while(str[i] != '\0') /* While we didn't reached the end */
    {
        char c = str[i];
        if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) /* If were're in [A-Z]|[a-z] */
        {
            str[i] = decode(toUpperChar(key[i % keyLength]), c);
        }
        i++;
    }
}

static char encode(char key, char decoded)
{
    int shift = ((int) key) - 'A';
    if(decoded >= 'a' && decoded <= 'z')
    {
        return (char) ('a' + (decoded - 'a' + shift) % 26); /* Shift over small case letters */
    }
    return (char) ('A' + (decoded - 'A' + shift) % 26); /* Shift over upper case letters */
}

static char decode(char key, char encoded)
{
    int shift = ((int) key) - 'A';
    if(encoded >= 'a' && encoded <= 'z')
    {
        return (char) ('a' + (encoded - 'a' - shift + 26) % 26); /* Shift back over small case letters */
    }
    return (char) ('A' + (encoded - 'A' - shift + 26) % 26); /* Shift back over upper case letters */
}
