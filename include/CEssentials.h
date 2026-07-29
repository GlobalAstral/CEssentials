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

char* CE__converrno(int err);

typedef unsigned char byte;

typedef struct CE__Iterator *CE__Iterator;

size_t CE__IteratorIndex(CE__Iterator it);
size_t CE__IteratorLength(CE__Iterator it);
bool CE__IteratorNext(CE__Iterator it);
void* CE__IteratorGet(CE__Iterator it);
void CE__FreeIterator(CE__Iterator it);

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
CE__Iterator CE__strrbegin(CE__String str);
char CE__strbyteat(CE__String str, size_t index);
char* CE__strat(CE__String str, size_t index);

typedef struct CE__ArrayList *CE__ArrayList, *CE__Stack;

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
CE__Iterator CE__ArrayListRBegin(CE__ArrayList self);
void* CE__ArrayListAt(CE__ArrayList self, size_t index);
void* CE__popArrayList(CE__ArrayList self);

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
CE__Iterator CE__LinkedListRBegin(CE__LinkedList ll);
void* CE__LinkedListGet(CE__LinkedList ll, size_t index);

size_t CE__StackLen(CE__Stack stack);
size_t CE__StackEleSz(CE__Stack stack);
CE__Stack CE__newStack(size_t element_size);
void CE__freeStack(CE__Stack stack);
int CE__pushStack(CE__Stack stack, void* element);
void* CE__popStack(CE__Stack stack);
void* CE__peekStack(CE__Stack stack);
CE__Iterator CE__StackBegin(CE__Stack stack);
CE__Iterator CE__StackRBegin(CE__Stack stack);

typedef struct CE__BitField *CE__BitField;

CE__BitField CE__newBitFieldEx(unsigned long long init);
CE__BitField CE__newBitField();
void CE__freeBitField(CE__BitField self);
bool CE__getBitField(CE__BitField self, unsigned char bit);
void CE__setBitField(CE__BitField self, unsigned char bit, bool flag);
void CE__clearBitField(CE__BitField self);
void CE__toggleBitField(CE__BitField self, unsigned char bit);

typedef struct CE__BitArray *CE__BitArray;

CE__BitArray CE__newBitArray(size_t size);
void CE__freeBitArray(CE__BitArray self);
bool CE__getBitArray(CE__BitArray self, size_t bit);
void CE__setBitArray(CE__BitArray self, size_t bit, bool flag);
void CE__clearBitArray(CE__BitArray self);
void CE__toggleBitArray(CE__BitArray self, size_t bit);

//TODO Logger
//TODO HashMap
//TODO HashSet
