#pragma once

#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;

typedef struct CE__String {
  byte* buffer;
  size_t capacity;
  size_t bytelen;
  size_t length;
} CE__String;

size_t CE_utf8len(CE__String* s);
CE__String CE_newString(char* init);
void CE_freeString(CE__String* s);
int CE_strRealloc(CE__String* s, size_t size);
size_t CE_utf8_byte_index(CE__String* target, size_t index);
int CE_insertString(CE__String* self, size_t index, CE__String* other);
