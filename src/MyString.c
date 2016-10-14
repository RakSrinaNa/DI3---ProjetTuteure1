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
 * $Id: MyString.c 293 2011-10-02 15:05:44Z sebtic $
 */

#include <MyString.h>
#include <limits.h>

/** Like the tolower() function. It converts the letter c to lower case, if possible.
 * @param c the letter to convert
 * @return the lower case letter associated to c if c is a letter, or c otherwise
 */
char IMPLEMENT(toLowerChar)(char c) {
    if (c >= 'A' && c <= 'Z') /* If we're in the range [A-Z] */
    {
        return (char) (c - 'A' + 'a'); /* Shift to lower char */
    }
    return c;
}

/** Like the toupper() function. It converts the letter c to upper case, if possible.
 * @param c the letter to convert
 * @return the upper case letter associated to c if c is a letter, or c otherwise
 */
char IMPLEMENT(toUpperChar)(char c) {
    if (c >= 'a' && c <= 'z') /* If we're in the range [a-z] */
    {
        return (char) (c - 'a' + 'A'); /* Shift to upper case */
    }
    return c;
}

/** Like the strcmp() function. It compares the two strings str1 and str2.
 * It returns an integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than, to match, or be greater
 * than str2.
 * @param str1 the first string
 * @param str2 the second string
 * @return an integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than, to match, or be greater
 * than str2
 */
int IMPLEMENT(compareString)(const char * str1, const char * str2) {
    char c1, c2;
    int i = 0;
    do
    {
        c1 = str1[i];
        c2 = str2[i];
        if (c1 != c2) /* If the characters are different */
        {
            return c1 - c2;
        }
        i++;
    } while (c1 != '\0' && c2 != '\0'); /* This condition is false only when the reached the end of both strings at the same time. Il only one ended we would have entered the above if */
    return 0; /* Only reached if all characters were the same, including '\0' */
}

/** Like the strcasecmp() function. It compares the two strings str1 and str2, ignoring the case of the characters.
 * It returns an integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than, to match, or be greater
 * than str2.
 * @param str1 the first string
 * @param str2 the second string
 * @return an integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than, to match, or be greater
 * than str2
 */
int IMPLEMENT(icaseCompareString)(const char * str1, const char * str2) {
    char c1, c2;
    int i = 0;
    do
    {
        c1 = toLowerChar(str1[i]);
        c2 = toLowerChar(str2[i]);
        if (c1 != c2) /* If the characters are different */
        {
            return c1 - c2;
        }
        i++;
    } while (c1 != '\0' && c2 != '\0'); /* This condition is false only when the reached the end of both strings at the same time. Il only one ended we would have entered the above if */
    return 0; /* Only reached if all characters were the same, including '\0' */
}

/** Like the strlen() function. It calculates the length of the string str, not including the terminating '\\0' character.
 * @param str the string
 * @return the number of characters in str.
 */
size_t IMPLEMENT(stringLength)(const char * str) {
    int count = 0;
    while (str[count] != '\0') /* While we didn't reach the end */
    {
        count++;
    }
    return (size_t) count;
}

/** Copy the string pointed to by src, including the terminating null byte ('\\0'), to the buffer pointed to by dest.
 * @warning The strings may not overlap, and the destination string dest must be large enough to receive the copy.
 *
 * @param dest the destination string
 * @param src the source string
 */
void IMPLEMENT(copyString)(char * dest, const char * src) {
    fprintf(stderr,
            "You should avoid strcpy and copyString. Use strncpy or copyStringWithLength instead\n");
    copyStringWithLength(dest, src, INT_MAX);
}

/** Copy the first destSize characters of the string pointed to by src, including the terminating null byte ('\\0'), to the buffer pointed to by dest.
 * @warning The strings may not overlap, and the destination string dest must be large enough to receive the copy.
 * @warning If src is longer than destSize characters, the string is truncated and the terminating null byte ('\\0') is added.
 *
 * @param dest the destination string
 * @param src the source string
 * @param destSize the maximal number of characters to copy
 */
void IMPLEMENT(copyStringWithLength)(char * dest, const char * src, size_t destSize) {
    int i = 0;
    while (i < (int) (destSize - 1) && src[i] != '\0') /* While we copied enough characters or that there is nothing left to copy */
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

/** Like the strdup() function. It creates a copy of the string on the heap.
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @param str the string to duplicate
 */
char * IMPLEMENT(duplicateString)(const char * str) {
    size_t length = stringLength(str) + 1;
    char *string;
    if ((string = (char *) malloc(length * sizeof(char))) == NULL)
    {
        printf("Erreur allocation malloc");
        exit(1);
    }
    copyStringWithLength(string, str, length);
    return string;
}

/** Test if the string str begins by the string start, ignoring the case of the characters.
 * @param start the string which should be at the beginning of str
 * @param str the string to test
 * @return a non null number (true) if the str begins with the string start, false otherwise
 */
int IMPLEMENT(icaseStartWith)(const char * start, const char * str) {
    int i = 0;
    while (start[i] != '\0' && str[i] != '\0') /* While we didn't reached the end of a string */
    {
        if (toLowerChar(start[i]) != toLowerChar(str[i])) /* If the characters are different */
        {
            return 0; /* We didn't found it */
        }
        i++;
    }
    if (start[i] == '\0') /* If we searched all the start string */
    {
        return 1; /* We found it */
    }
    return 0; /* Else the str ended before start, so we didn't found it */
}

/** Test if the string str ends by the string start, ignoring the case of the characters.
 * @param end the string which should be at the end of str
 * @param str the string to test
 * @return a non null number (true) if the str ends with the string end, false otherwise
 */
int IMPLEMENT(icaseEndWith)(const char * end, const char * str) {
    unsigned int i = 0;
    size_t endLength = stringLength(end);
    size_t strLength = stringLength(str);
    size_t base = strLength - endLength;
    if ((int) base < 0) /* If the end string is longer than the string itself */
    {
        return 0;
    }
    while (end[i] != '\0') /* While we didn't reached the end of a the string */
    {
        if (toLowerChar(end[i]) != toLowerChar(str[base + i])) /* If the characters are different */
        {
            return 0; /* We didn't found it */
        }
        i++;
    }
    return 1; /* We searched all the end string, so we found it */
}

/** Create a new string on the heap which is the result of the concatenation of the two strings.
 * @note The new string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @param str1 the first string
 * @param str2 the second string
 * @return the new string
 */
char * IMPLEMENT(concatenateString)(const char * str1, const char * str2) {
	int i = 0;
	size_t length1 = stringLength(str1);
	size_t length2 = stringLength(str2);
	char *string;
	if ((string = (char *) malloc((length1 + length2 + 1) * sizeof(char))) == NULL)
	{
		printf("Erreur allocation malloc");
		exit(1);
	}
	while(*str1 != '\0')
	{
		*(string + i) = *str1;
		i++;
		str1++;
	}
	while(*str2 != '\0')
	{
		*(string + i) = *str2;
		i++;
		str2++;
	}
	string[i] = '\0';
	return string;
}

/** Like the index() function. It returns a pointer to the first occurrence of the character c in the string str.
 * @param str the string to search in
 * @param c the character to find
 * @return a pointer to the first occurrence of the character c in the string str if c is in str, NULL otherwise
 */
const char * IMPLEMENT(indexOfChar)(const char *str, char c) {
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == c)
        {
            return &str[i];
        }
        i++;
    }
    return NULL;
}

/** Create a copy on the heap of part of a string. The new string contains the characters pointed from start (inclusive) to end (exclusive).
 * @note The new string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @param start a pointer to the first character of the new string
 * @param end a pointer to the next character of the new string (the pointed character is not part of the new string)
 * @return the new string
 *
 * @code
 * char * str = "abcdef";
 * char * s1 = subString(str,str);
 * char * s2 = subString(str,str+strlen(str));
 * char * s3 = subString(str+1,str+2);
 *
 * We get:
 *    s1: "";
 *    s2: "abcdef";
 *    s3: "b";
 * @endcode
 */
char * IMPLEMENT(subString)(const char * start, const char * end) {
    unsigned int i = 0;
    char *string;
    if ((string = (char *) malloc((long unsigned int)(end - start + 1) * sizeof(char))) == NULL)
    {
        printf("Erreur allocation malloc");
        exit(1);
    }
    while ((start + i) != end) /* While we didn't reached the end */
    {
        string[i] = start[i];
        i++;
    }
    string[i] = '\0';
    return string;
}

/** Like the strstr() function. It returns a pointer to the first occurrence of the string aiguille in the string meule_de_foin.
 * @param meule_de_foin the string to search in
 * @param aiguille the string to find
 * @return a pointer to the first occurrence of the string aiguille in the string meule_de_foin if aiguille is in meule_de_foin, NULL otherwise
 */
const char * IMPLEMENT(indexOfString)(const char *meule_de_foin, const char *aiguille) {
    int i = 0;
    while (meule_de_foin[i] != '\0') /* While we didn't hit the end of the mail string */
    {
        if (meule_de_foin[i] == aiguille[0]) /* If we found the beginning of the string to search */
        {
            int j = 1;
            while (meule_de_foin[i + j] != '\0' && aiguille[j] != '\0') /* While non of the strings hit the end */
            {
                if (meule_de_foin[i + j] != aiguille[j]) /* If the character found isn't the same, we stop */
                {
                    break;
                }
                j++;
            }
            if (aiguille[j] == '\0') /* If we searched up the the end of the string to find, we found it */
            {
                return &meule_de_foin[i];
            }
        }
        i++;
    }
    return NULL;
}

/** Convert a string to upper case.
 * @param str the string to convert
 */
void IMPLEMENT(makeUpperCaseString)(char * str) {
    char *string = str;
    while (*string != '\0')
    {
        *string = toUpperChar(*string);
        string++;
    }
}

/** Convert a string to lower case.
 * @param str the string to convert
 */
void IMPLEMENT(makeLowerCaseString)(char * str) {
    char *string = str;
    while (*string != '\0')
    {
        *string = toLowerChar(*string);
        string++;
    }
}

/** Create a new string on the heap which contents is the result of the insertion in src of insertLength characters from  toBeInserted at position insertPosition.
 * @note The new string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @param src the string in which the insertion is done
 * @param insertPosition the position where the insertion is done
 * @param toBeInserted a pointer on the first character to insert
 * @param insertLength the number of character to insert
 * @return the new string
 */
char * IMPLEMENT(insertString)(const char * src, int insertPosition, const char * toBeInserted,
        int insertLength) {
    size_t strLength = stringLength(src);
    char *string;
    if ((string = (char *) malloc((strLength + (unsigned int)insertLength + 1) * sizeof(char))) == NULL)
    {
        printf("Erreur allocation malloc");
        exit(1);
    }
    copyStringWithLength(string, src, (size_t) (insertPosition + 1)); /* Copy before insert */
    copyStringWithLength(string + insertPosition, toBeInserted, (size_t)(insertLength + 1)); /* Copy insert */
    copyStringWithLength(string + insertPosition + insertLength, src + insertPosition, (size_t)(strLength - (unsigned int)insertPosition + 1)); /* Copy after insert */
    return string;
}
