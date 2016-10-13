#include <printf.h>
#include <stdlib.h>
#include "MyString.h"
#include "DocumentUtil.h"

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
    switch(c)
    {
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'A':
            return 10;
        case 'B':
            return 11;
        case 'C':
            return 12;
        case 'D':
            return 13;
        case 'E':
            return 14;
        case 'F':
            return 15;
        case '0':
        default:
            return 0;
    }
}
