#ifndef _misc_h
#define _misc_h

#include "Base.h"

#define HEX 1

unsigned long str2int(char *string);

int str2cmd(char *command);
float ul2float(unsigned long ul);
void PrintHex8(uint8_t *data, uint8_t length);
void PrintHexByte(uint8_t data);
byte reverseBits(byte data);

#endif
