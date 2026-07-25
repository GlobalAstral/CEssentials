#pragma once

typedef bool (*CE__Iterator__NextFunc)(CE__Iterator*);
typedef void* (*CE__Iterator__GetFunc)(CE__Iterator*);

typedef struct IteratorSecret {
  byte* pointer;
  size_t step;

  CE__Iterator__NextFunc next;
  CE__Iterator__GetFunc get;
} *IteratorSecret;

IteratorSecret newITS(struct IteratorSecret secret);
