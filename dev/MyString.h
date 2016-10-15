#include <ntsid.h>

#ifndef TP2_MYSTRING_H_H
#define TP2_MYSTRING_H_H

char toLowerChar(char c);
char toUpperChar(char c);
void makeLowerCaseString(char * str);
void makeUpperCaseString(char * str);

size_t stringLength(const char * str);
size_t recStringLength(const char * str);

int compareString(const char * str1, const char * str2);
int icaseCompareString(const char * str1, const char * str2);

const char * indexOfChar(const char * str, char c);
const char * indexOfString(const char * meule_de_foin, const char * aiguille);
int icaseStartsWith(const char * start, const char * str);
int icaseEndWith(const char * end, const char * str);

void copyStringWithLength(char * dest, const char * src, size_t destSize);
char * duplicateString(const char * str);
char * concatenateString(const char * str1, const char * str2);
char * subString(const char * start, const char * end);
char * insertString(const char * src, int insertPosition, const char * toBeInserted, int insertLength);
void insertInString(char * const src, int insertPosition, const char * toBeInserted, unsigned int insertLength);

unsigned int charInInt(const int number);
int getNumberValue(const char c);

#endif
