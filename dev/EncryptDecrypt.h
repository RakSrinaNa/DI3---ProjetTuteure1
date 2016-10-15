#ifndef PROJETTUTORE1_ENCRYPTDECRYPT_H
#define PROJETTUTORE1_ENCRYPTDECRYPT_H

void encrypt(const char * key, char * str);
void decrypt(const char * key, char * str);
char encode(char key, char decoded);
char decode(char key, char encoded);

int mod(const int a, const int b);

#endif
