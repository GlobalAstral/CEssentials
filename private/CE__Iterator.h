#pragma once

#include <CEssentials.h>

typedef bool (*CE__Iterator__NextFunc)(CE__Iterator);
typedef void* (*CE__Iterator__GetFunc)(CE__Iterator);

struct CE__Iterator {
  size_t index;
  size_t length;
  byte* pointer;
  size_t step;
  size_t capacity;
  
  CE__Iterator__NextFunc next;
  CE__Iterator__GetFunc get;
};
