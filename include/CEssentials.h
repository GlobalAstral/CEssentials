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
#define NOT_FOUND -8

#define CE_ANSI_RESET "\033[0m"

// Styles
#define CE_ANSI_BOLD          "\033[1m"
#define CE_ANSI_DIM           "\033[2m"
#define CE_ANSI_ITALIC        "\033[3m"
#define CE_ANSI_UNDERLINE     "\033[4m"
#define CE_ANSI_BLINK         "\033[5m"
#define CE_ANSI_REVERSE       "\033[7m"
#define CE_ANSI_HIDDEN        "\033[8m"
#define CE_ANSI_STRIKETHROUGH "\033[9m"

// Normal foreground colors
#define CE_ANSI_BLACK   "\033[30m"
#define CE_ANSI_RED     "\033[31m"
#define CE_ANSI_GREEN   "\033[32m"
#define CE_ANSI_YELLOW  "\033[33m"
#define CE_ANSI_BLUE    "\033[34m"
#define CE_ANSI_MAGENTA "\033[35m"
#define CE_ANSI_CYAN    "\033[36m"
#define CE_ANSI_WHITE   "\033[37m"

// Bright foreground colors
#define CE_ANSI_BRIGHT_BLACK   "\033[90m"
#define CE_ANSI_BRIGHT_RED     "\033[91m"
#define CE_ANSI_BRIGHT_GREEN   "\033[92m"
#define CE_ANSI_BRIGHT_YELLOW  "\033[93m"
#define CE_ANSI_BRIGHT_BLUE    "\033[94m"
#define CE_ANSI_BRIGHT_MAGENTA "\033[95m"
#define CE_ANSI_BRIGHT_CYAN    "\033[96m"
#define CE_ANSI_BRIGHT_WHITE   "\033[97m"

// Normal background colors
#define CE_ANSI_BG_BLACK   "\033[40m"
#define CE_ANSI_BG_RED     "\033[41m"
#define CE_ANSI_BG_GREEN   "\033[42m"
#define CE_ANSI_BG_YELLOW  "\033[43m"
#define CE_ANSI_BG_BLUE    "\033[44m"
#define CE_ANSI_BG_MAGENTA "\033[45m"
#define CE_ANSI_BG_CYAN    "\033[46m"
#define CE_ANSI_BG_WHITE   "\033[47m"

// Bright background colors
#define CE_ANSI_BG_BRIGHT_BLACK   "\033[100m"
#define CE_ANSI_BG_BRIGHT_RED     "\033[101m"
#define CE_ANSI_BG_BRIGHT_GREEN   "\033[102m"
#define CE_ANSI_BG_BRIGHT_YELLOW  "\033[103m"
#define CE_ANSI_BG_BRIGHT_BLUE    "\033[104m"
#define CE_ANSI_BG_BRIGHT_MAGENTA "\033[105m"
#define CE_ANSI_BG_BRIGHT_CYAN    "\033[106m"
#define CE_ANSI_BG_BRIGHT_WHITE   "\033[107m"

#ifdef _WIN32
  #define PATH_SEPARATOR '\\'
#else
  #define PATH_SEPARATOR '/'
#endif

#define nullptr NULL
#define loop while(true)
#define guard(condition, ret) if ((condition)) return (ret)
#define castderef(type, expr) (*((type*)(expr)))
#define min(a, b) (((a) < (b)) ? (a) : (b)) 
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define clamp(a, min, max) ((((a) < (min)) ? (min)) : (((a) > (max)) ? (max) : (a)))

const char* const CE__converrno(int err);
void* CE__memdup(void* mem, size_t size);
int CE__memswap(void* mem1, void* mem2, size_t size);

unsigned long long CE__alignUp(unsigned long long num, unsigned long long alignment);
bool CE__isAligned(unsigned long long num, unsigned long long alignment);

bool CE__fileExists(char* path);
char* CE__getFilename(char* path);
char* CE__getExtension(char* path);

typedef void* (*CE__Malloc)(size_t size);
typedef void* (*CE__Realloc)(void* buf, size_t size);
typedef void* (*CE__Calloc)(size_t size, size_t ele_size);
typedef void (*CE__Free)(void*);

void CE__selectAllocators(CE__Malloc m, CE__Realloc r, CE__Calloc c, CE__Free f);

void* CE__malloc(size_t size);
void* CE__realloc(void* buf, size_t size);
void* CE__calloc(size_t size, size_t ele_size);
void CE__free(void* buf);

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

typedef struct CE__Logger *CE__Logger;

CE__Logger CE__newLogger(FILE* stream);
CE__Logger CE__newLoggerPath(char* path);
void CE__freeLogger(CE__Logger self);
int CE__LoggerTrace(CE__Logger self, char* fmt, ...);
int CE__LoggerDebug(CE__Logger self, char* fmt, ...);
int CE__LoggerInfo(CE__Logger self, char* fmt, ...);
int CE__LoggerWarn(CE__Logger self, char* fmt, ...);
int CE__LoggerError(CE__Logger self, char* fmt, ...);
int CE__LoggerFatal(CE__Logger self, char* fmt, ...);

typedef struct CE__Hash128 *CE__Hash128;

CE__Hash128 CE__hash128(void* data, size_t size);
void CE__freeHash128(CE__Hash128 self);
bool CE__Hash128Equals(CE__Hash128 a, CE__Hash128 b);
unsigned long long CE__Hash128Modulo(CE__Hash128 self, unsigned long long mod);

typedef bool (*CE__HashMapEquals)(void*, void*, size_t);

typedef struct CE__HashMap *CE__HashMap;

size_t CE__lengthHashMap(CE__HashMap self);
CE__HashMap CE__newHashMapEx(size_t key_size, size_t value_size, CE__HashMapEquals equals);
CE__HashMap CE__newHashMap(size_t key_size, size_t value_size);
void CE__freeHashMap(CE__HashMap self);
int CE__insertHashMap(CE__HashMap self, void* key, void* value);
void* CE__getHashMap(CE__HashMap self, void* key);
bool CE__containsHashMap(CE__HashMap self, void* key);
int CE__removeHashMap(CE__HashMap self, void* key);
int CE__reserveHashMap(CE__HashMap self, size_t amount);
void* CE__getOrCreateHashMap(CE__HashMap self, void* key, void* value);

//TODO HashSet

void CE__randomSeed(unsigned long long seed);
void CE__randomSeedTime();
unsigned int CE__randomUInt32();
unsigned long long CE__randomUInt64();

int CE__randomInt(int min, int max);
size_t CE__randomSize(size_t min, size_t max);

float CE__randomFloat();
double CE__randomDouble();

bool CE__randomBool();

void CE__randomBytes(void* buffer, size_t size);
size_t CE__randomIndex(size_t size);
