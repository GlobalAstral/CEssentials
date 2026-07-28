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
#define CANNOT_ALLOCATE -5
#define INDEX_OUT_OF_BOUNDS -6
#define TYPE_EMPTY -7

#define nullptr NULL
#define loop while(true)
#define guard(condition, ret) if ((condition)) return (ret)
#define castderef(type, expr) (*((type*)(expr)))

typedef unsigned char byte;

typedef struct CE__Iterator {
  size_t index;
  size_t length;
  void* __internal;
} CE__Iterator;

bool CE__IteratorNext(CE__Iterator* it);
void* CE__IteratorGet(CE__Iterator* it);
void CE__FreeIterator(CE__Iterator* it);

typedef struct CE__String *CE__String;

size_t CE__strlength(CE__String s);
size_t CE__strblen(CE__String s);
size_t CE__utf8_char_size(byte b);
size_t CE__utf8len(CE__String s);
CE__String CE__newString(char* init);
CE__String CE__newStringLen(char* init, size_t size);
void CE__freeString(CE__String s);
int CE__strRealloc(CE__String s, size_t size);
size_t CE__utf8_byte_index(CE__String target, size_t index);
int CE__insertString(CE__String self, size_t index, CE__String other);
int CE__insertCstr(CE__String self, size_t index, char* s);
int CE__appendString(CE__String self, CE__String other);
int CE__appendCstr(CE__String self, char* other);
int CE__strcmp(CE__String self, CE__String other);
bool CE__strequ(CE__String self, CE__String other);
char* CE__strcstr(CE__String self);
CE__String CE__substr(CE__String self, size_t start, size_t end);
CE__String CE__strfind(CE__String self, CE__String find);
bool CE__strcontains(CE__String self, CE__String find);
int CE__printstr(CE__String self);
int CE__fprintstr(FILE* stream, CE__String self);
int CE__strdrain(CE__String self, size_t start, size_t end);
CE__Iterator CE__strbegin(CE__String str);

typedef struct CE__ArrayList *CE__ArrayList;

size_t CE__ArrayListLen(CE__ArrayList self);
size_t CE__ArrayListEleSz(CE__ArrayList self);
CE__ArrayList CE__newArrayList(size_t element_size);
void CE__freeArrayList(CE__ArrayList list);
int CE__ArrayListRealloc(CE__ArrayList list, size_t size);
int CE__insertArrayList(CE__ArrayList self, size_t index, void* item);
int CE__appendArrayList(CE__ArrayList self, void* item);
CE__ArrayList CE__ArrayListSection(CE__ArrayList self, size_t start, size_t end);
void* CE__ArrayListFind(CE__ArrayList self, void* item);
bool CE__ArrayListContains(CE__ArrayList self, void* item);
int CE__removeArrayList(CE__ArrayList self, size_t index);
CE__Iterator CE__ArrayListBegin(CE__ArrayList self);

typedef struct CE__LinkedList *CE__LinkedList;

size_t CE__LinkedListLen(CE__LinkedList ll);
CE__LinkedList CE__newLinkedList(size_t element_size);
void CE__freeLinkedList(CE__LinkedList ll);
int CE__insertLinkedList(CE__LinkedList ll, size_t index, void* element);
int CE__appendLinkedList(CE__LinkedList ll, void* element);
void* CE__LinkedListFind(CE__LinkedList ll, void* item);
bool CE__LinkedListContains(CE__LinkedList ll, void* item);
int CE__removeLinkedList(CE__LinkedList ll, size_t index);
CE__Iterator CE__LinkedListBegin(CE__LinkedList ll);

//TODO MAKE ALL OTHER STRUCTURE POINTER TYPES. MAKE EVERYTHING OPAQUE AND EXPOSE FUNCTIONS. MAKE ITERATOR BEGIN REVERSED AND PREV
