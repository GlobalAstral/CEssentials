#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned char byte;

typedef struct CE__String {
  byte* buffer;
  size_t capacity;
  size_t bytelen;
  size_t length;
} CE__String;

size_t CE__utf8len(CE__String* s);

CE__String CE__newString(char* init);
CE__String CE__newStringLen(char* init, size_t size);

void CE__freeString(CE__String* s);

int CE__strRealloc(CE__String* s, size_t size);

size_t CE__utf8_byte_index(CE__String* target, size_t index);

int CE__insertString(CE__String* self, size_t index, CE__String* other);
int CE__insertCstr(CE__String* self, size_t index, char* s);
int CE__appendString(CE__String* self, CE__String* other);
int CE__appendCstr(CE__String* self, char* other);

int CE__strcmp(CE__String* self, CE__String* other);
bool CE__strequ(CE__String* self, CE__String* other);

char* CE__strcstr(CE__String* self);

CE__String CE__substr(CE__String* self, size_t start, size_t end);

CE__String* CE__strfind(CE__String* self, CE__String* find);
bool CE__strcontains(CE__String* self, CE__String* find);

int CE__printstr(CE__String* self);
int CE__fprintstr(FILE* stream, CE__String* self);
