#ifndef PROJETTUTORE1_DOCUMENTUTIL_H
#define PROJETTUTORE1_DOCUMENTUTIL_H

char * formatDate(int date, int month, int year);
void intToString(const int number, char * const string, size_t stringLength, const int fillZeros);
long power(const int base, const int power);

long BaseB2Dec(int B, int N, int tab[]);
long Bin2Dec(int tab[]);
long Hex2Dec(const char * hex);

char * computeDocumentNumber(long id);

#endif
