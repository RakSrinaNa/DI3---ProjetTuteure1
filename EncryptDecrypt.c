#include "EncryptDecrypt.h"
#include "MyString.h"

void encrypt(const char * key, char * str)
{
    size_t keyLength = stringLength(key);
    int i = 0;
    while(str[i] != '\0') /* While we didn't reached the end */
    {
        char c = str[i];
        if(c >= 65 && c <= 90 || c >=97 && c <= 122) /* If were're in [A-Z]|[a-z] */
        {
            str[i] = encode(toUpperChar(key[i % keyLength]), c);
        }
        i++;
    }
}

void decrypt(const char * key, char * str)
{
    size_t keyLength = stringLength(key);
    int i = 0;
    while(str[i] != '\0') /* While we didn't reached the end */
    {
        char c = str[i];
        if(c >= 65 && c <= 90 || c >= 97 && c <= 122) /* If were're in [A-Z]|[a-z] */
        {
            str[i] = decode(toUpperChar(key[i % keyLength]), c);
        }
        i++;
    }
}

char encode(char key, char decoded)
{
    int shift = ((int) key) - 65;
    if(decoded >= 65 && decoded <= 90)
    {
        return (char) (65 + (decoded - 65 + shift) % 26); /* Shift over small case letters */
    }
    return (char) (97 + (decoded - 97 + shift) % 26); /* Shift over upper case letters */
}

char decode(char key, char encoded)
{
    int shift = ((int) key) - 65;
    if(encoded >= 65 && encoded <= 90)
    {
        return (char) (65 + mod(encoded - 65 - shift, 26)); /* Shift back over small case letters */
    }
    return (char) (97 + mod(encoded - 97 - shift, 26)); /* Shift back over upper case letters */
}

int mod (const int a, const int b)
{
    int leftover = a % b;
    if(leftover < 0) /* If the leftover of the euclid's division is negative */
    {
        leftover += b; /* Add the base to get back to a positive number, the modulo */
    }
    return leftover;
}
