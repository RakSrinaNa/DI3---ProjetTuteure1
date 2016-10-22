#include <stddef.h>
#include <stdlib.h>
#include <printf.h>
#include "MyString.h"
#include "DocumentUtil.h"

void exchangeChar (char *string, char *string1);

/**
 * Returns the lower case of a character.
 *
 * @param c The character to process.
 * @return The lower case of c.
 */
char toLowerChar (char c)
{
	if (c >= 'A' && c <= 'Z') /* If we're in the range [A-Z] */
	{
		return (char) (c - 'A' + 'a'); /* Shift to lower char */
	}
	return c;
}

/**
 * Returns the upper case of a character.
 *
 * @param c The character to process.
 * @return The upper case of c.
 */
char toUpperChar (char c)
{
	if (c >= 'a' && c <= 'z') /* If we're in the range [a-z] */
	{
		return (char) (c - 'a' + 'A'); /* Shift to upper case */
	}
	return c;
}

/**
 * Makes the string all lower case.
 *
 * @param str The string to process.
 */
void makeLowerCaseString (char *str)
{
	char *string = str;
	while (*string != '\0')
	{
		*string = toLowerChar(*string);
		string++;
	}
}

/**
 * Makes the string all upper case.
 *
 * @param str The string to process.
 */
void makeUpperCaseString (char *str)
{
	char *string = str;
	while (*string != '\0')
	{
		*string = toUpperChar(*string);
		string++;
	}
}

/**
 * Return the length of a string, excluding '\0'.
 *
 * @param str The string to process.
 * @return The length of the string.
 */
size_t stringLength (const char *str)
{
	int count = 0;
	while (str[count] != '\0') /* While we didn't reach the end */
	{
		count++;
	}
	return (size_t) count;
}

/**
 * Return the length of a string, excluding '\0'.
 *
 * @param str The string to process.
 * @return The length of the string.
 */
size_t recStringLength (const char *str)
{
	if (*str == '\0')
	{
		return (size_t) 0;
	}
	return (size_t) (1 + (int) recStringLength(str + 1)); /* Counts the current character and add the length of the string after our character */
}

/**
 * Compare two strings.
 *
 * @param str1 1st string to compare.
 * @param str2 2nd string to compare.
 * @return A negative integer if str1 is before str2, 0 if they are equals, a positive integer if str1 is after str2.
 */
int compareString (const char *str1, const char *str2)
{
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

/**
 * Compare two strings without taking the case into account.
 *
 * @param str1 1st string to compare.
 * @param str2 2nd string to compare.
 * @return A negative integer if str1 is before str2, 0 if they are equals, a positive integer if str1 is after str2.
 */
int icaseCompareString (const char *str1, const char *str2)
{
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

/**
 * Searches the first appearance of a char in  string.
 *
 * @param str The string in which to search.
 * @param c The character to find.
 * @return NULL if the character was not found, else a pointer on the found character.
 */
const char *indexOfChar (const char *str, char c)
{
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

/**
 * Searches the first appearance of a string in  string.
 *
 * @param meule_de_foin The string in which to search.
 * @param aiguille The string to find.
 * @return NULL if the string was not found, else a pointer on the found string.
 */
const char *indexOfString (const char *meule_de_foin, const char *aiguille)
{
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

/**
 * Test if the string starts with another string.
 *
 * @param start The starting string we want to search.
 * @param str The string in which the search is done.
 * @return 1 if str starts with start, 0 else.
 */
int icaseStartsWith (const char *start, const char *str)
{
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

/**
 * Test if the string ends with another string.
 *
 * @param start The ending string we want to search.
 * @param str The string in which the search is done.
 * @return 1 if str ends with start, 0 else.
 */
int icaseEndWith (const char *end, const char *str)
{
	int i = 0;
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

/**
 * Copy a maximum of destSize characters from src into dest.
 *
 * @param dest Where the characters will be copied.
 * @param src The characters to copy.
 * @param destSize The maximum of characters to copy.
 */
void copyStringWithLength (char *dest, const char *src, size_t destSize)
{
	int i = 0;
	while (i < (int) (destSize - 1) && src[i] != '\0') /* While we copied enough characters or that there is nothing left to copy */
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

/**
 * Duplicate the string str in the heap.
 *
 * @param str The string to duplicate.
 * @return The duplicated str in the heap.
 */
char *duplicateString (const char *str)
{
	size_t length = stringLength(str) + 1;
	char *result;
	if ((result = (char *) malloc(length * sizeof(char))) == NULL)
	{
		printf("Erreur allocation malloc");
		exit(1);
	}
	copyStringWithLength(result, str, length);
	return result;
}

/**
 * Concatenate the strings in the heap.
 *
 * @param str1 The first string to concatenate.
 * @param str2 The second string to concatenate.
 * @return The concatenated string in the heap.
 */
char *concatenateString (const char *str1, const char *str2)
{
	size_t length1 = stringLength(str1);
	size_t length2 = stringLength(str2);
	char *string;
	if ((string = (char *) malloc((length1 + length2 + 1) * sizeof(char))) == NULL)
	{
		printf("Erreur allocation malloc");
		exit(1);
	}
	copyStringWithLength(string, str1, length1);
	copyStringWithLength(string + length1, str2, length2);
	return string;
}

/**
 * Substring a string in the heap.
 *
 * @param start The first starting character.
 * @param end The ending character (excluded).
 * @return The substring in the heap.
 */
char *subString (const char *start, const char *end)
{
	int i = 0;
	char *string;
	if ((string = (char *) malloc((end - start + 1) * sizeof(char))) == NULL)
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

/**
 * Inserts a string into another.
 *
 * @param src The string in which the other string will be inserted.
 * @param insertPosition The position to insert the string.
 * @param toBeInserted The string to insert.
 * @param insertLength The maximum of characters of toBeInserted to insert.
 * @return A new string in the heap with the toBeInserted string inserted into src.
 */
char *insertString (const char *src, int insertPosition, const char *toBeInserted, int insertLength)
{
	size_t strLength = stringLength(src);
	char *string;
	if ((string = (char *) malloc((strLength + insertLength + 1) * sizeof(char))) == NULL)
	{
		printf("Erreur allocation malloc");
		exit(1);
	}
	copyStringWithLength(string, src, (size_t) insertPosition + 1); /* Copy before insert */
	copyStringWithLength(string + insertPosition, toBeInserted, (size_t) insertLength + 1); /* Copy insert */
	copyStringWithLength(string + insertPosition + insertLength, src + insertPosition, strLength - insertPosition + 1); /* Copy after insert */
	return string;
}

void insertInString(char * const src, int insertPosition, const char * toBeInserted, unsigned int insertLength)
{
	size_t strLength = stringLength(src);
	size_t insertRealLength = (insertPosition + insertLength) > strLength ? (strLength - insertPosition) : insertLength;
	char before = src[insertPosition + insertRealLength];
	copyStringWithLength(src + insertPosition, toBeInserted, (size_t) insertRealLength + 1); /* Copy insert */
	src[insertPosition + insertRealLength] = before;
}

int getNumberValue(const char c)
{
	if(c >= '0' && c <= '9')
	{
		return c - '0';
	}
	if(c >= 'a' && c<= 'z')
	{
		return 10 + (c - 'a');
	}
	if(c >= 'A' && c<= 'Z')
	{
		return 10 + (c - 'A');
	}
	return 0;
}

unsigned int charInNumber (const long number)
{
	unsigned int base = 1;
	while((number / power(10, base)) >= 1)
	{
		base++;
	}
	if(number < 0)
	{
		base++;
	}
	return base;
}

char getCharNumber(const unsigned int number)
{
	if(number < 10)
	{
		return (char) ('0' + number);
	}
	return (char) ('A' + number - 10);
}

void reverseString(char * string, size_t length)
{
	int i;
	int maxIt = (int) ((length - 1) / 2);
	for(i = 0; i < maxIt; i++)
	{
		exchangeChar(&string[i], &string[length - 2 - i]);
	}
}

void exchangeChar (char * c1, char * c2)
{
	char temp = *c1;
	*c1 = *c2;
	*c2 = temp;
}
