#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define OK 0
#define VALUE_IS_NULL -1
#define OTHER_VALUE_IS_NULL -2
#define VALUE_IS_FREED -3
#define OTHER_VALUE_IS_FREED -4
#define CANNOT_ALLOCATE -4
#define INDEX_OUT_OF_BOUNDS -5
#define TYPE_EMPTY -6

#define nullptr NULL
#define loop while(true)
#define guard(condition, ret) if ((condition)) return (ret)
#define castderef(type, expr) (*((type*)(expr)))

typedef unsigned char byte;

typedef struct CE__Iterator CE__Iterator; 

typedef bool (*CE__Iterator__NextFunc)(CE__Iterator*);
typedef void* (*CE__Iterator__GetFunc)(CE__Iterator*);

typedef struct CE__Iterator {
  byte* pointer;
  size_t index;
  size_t length;
  size_t step;

  CE__Iterator__NextFunc next;
  CE__Iterator__GetFunc get;

} CE__Iterator;

typedef struct CE__String {
  byte* buffer;
  size_t capacity;
  size_t bytelen;
  size_t length;
  bool isfreed;
} CE__String, *CE__StrView;

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
CE__StrView CE__substr(CE__String* self, size_t start, size_t end);
CE__StrView CE__strfind(CE__String* self, CE__String* find);
bool CE__strcontains(CE__String* self, CE__String* find);
int CE__printstr(CE__String* self);
int CE__fprintstr(FILE* stream, CE__String* self);
int CE__strdrain(CE__String* self, size_t start, size_t end);
CE__Iterator CE__strbegin(CE__String* str);
bool CE__strnext(CE__Iterator* it);
void* CE__strget(CE__Iterator* it);

typedef struct CE__ArrayList {
  byte* buffer;
  size_t capacity;
  size_t length;
  size_t element_size;
  bool isfreed;
} CE__ArrayList, *CE__ArrayListView;

CE__ArrayList CE__newArrayList(size_t element_size);
void CE__freeArrayList(CE__ArrayList* list);
int CE__ArrayListRealloc(CE__ArrayList* list, size_t size);
int CE__insertArrayList(CE__ArrayList* self, size_t index, void* item);
int CE__appendArrayList(CE__ArrayList* self, void* item);
CE__ArrayListView CE__ArrayListSection(CE__ArrayList* self, size_t start, size_t end);
void* CE__ArrayListFind(CE__ArrayList* self, void* item);
bool CE__ArrayListContains(CE__ArrayList* self, void* item);
int CE__removeArrayList(CE__ArrayList* self, size_t index);
CE__Iterator CE__ArrayListBegin(CE__ArrayList* self);
bool CE__ArrayListNext(CE__Iterator* it);
void* CE__ArrayListGet(CE__Iterator* it);
