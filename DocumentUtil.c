#include <printf.h>
#include <stdlib.h>
#include "MyString.h"
#include "DocumentUtil.h"

char * formatDate(int date, int month, int year)
{
    char * dateFormat;
    char * partsBuffer;
    if((dateFormat = (char *) malloc(11 * sizeof(char))) == NULL)
    {
        printf("Erreur allocation malloc!\n");
        exit(1);
    }
    if((partsBuffer = (char *) malloc(5 * sizeof(char))) == NULL)
    {
        printf("Erreur allocation malloc!\n");
        exit(1);
    }
    intToString(year, partsBuffer, 5);
    insertString(dateFormat, 6, partsBuffer, 4);
    intToString(month, partsBuffer, 5);
    insertString(dateFormat, 3, partsBuffer, 2);
    intToString(date, partsBuffer, 5);
    insertString(dateFormat, 0, partsBuffer, 2);
    free(partsBuffer);
    return dateFormat;
}

void intToString(const int number, char const * string, size_t stringLength, const int fillZeros)
{
    int i = 0;
    int numberCopy = number;
    unsigned int intLength = charInInt(number);
    unsigned int maxCopy = (fillZeros || ((stringLength - 1) < intLength)) ? (stringLength - 1) : intLength;
    while(i < maxCopy)
    {
        string[maxCopy - i] = (char) ('0' + (numberCopy) % 10);
        numberCopy /= 10;
    }
    if(number < 0)
    {
        string[0] = '-';
    }
    string[maxCopy + 1] = '\0';
}

unsigned int charInInt(const int number)
{
    unsigned int base = 1;
    while((number / power(10, base)) < 10)
    {
        base++;
    }
    return base;
}

long power(const int base, const int power)
{
    int i;
    long result = 1;
    if((power == 0) || (base == 1))
    {
        return 1;
    }
    for(i = 0; i < power; i++)
    {
        result *= base;
    }
    return result;
}

long BaseB2Dec(int B, int N, int tab[])
{
    int i;
    long result = tab[0];
    for(i = 1; i < N; i++)
    {
        result = tab[i] + B * result;
    }
    return result;
}

long Bin2Dec(int tab[])
{
    long result = BaseB2Dec(2, 15, &tab[1]);
    if(tab[0] == 1)
    {
        result -= 32768;
    }
    return result;
}

long Hex2Dec(const char * hex)
{
    size_t hexLength = stringLength(hex);
    int * values;
    long result = 0;
    int i;
    if((values = (int *) malloc((hexLength - 2) * sizeof(int))) == NULL)
    {
        printf("Erreur allocation malloc");
        exit(1);
    }
    i = 2;
    if(hexLength < 3 || hex[0] != '0' || toLowerChar(hex[1]) != 'x') /* If string doesn't starts with '0x' */
    {
        printf("Hex number isn't formatted correctly!");
        exit(1);
    }
    while(hex[i] != '\0')
    {
        values[i - 2] = getNumberValue(toUpperChar(hex[i])); /* Build values array, converting A-F to integers */
        i++;
    }
    result = BaseB2Dec(16, (int) (hexLength - 2), values);
    free(values);
    return result;
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
