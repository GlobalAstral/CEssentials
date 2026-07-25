#include <CEssentials.h>

typedef bool (*CE__Iterator__NextFunc)(CE__Iterator*);
typedef void* (*CE__Iterator__GetFunc)(CE__Iterator*);

typedef struct IteratorSecret {
  byte* pointer;
  size_t step;

  CE__Iterator__NextFunc next;
  CE__Iterator__GetFunc get;
} *IteratorSecret;

IteratorSecret newITS(struct IteratorSecret secret) {
  IteratorSecret ret = (IteratorSecret)malloc(sizeof(struct IteratorSecret));
  *ret = secret;
  return ret;
}

bool CE__IteratorNext(CE__Iterator* it) {
  IteratorSecret secret = it->__internal;
  return secret->next(it);
}

void* CE__IteratorGet(CE__Iterator* it) {
  IteratorSecret secret = it->__internal;
  return secret->get(it);
}

void CE__IteratorDispose(CE__Iterator* it) {
  free(it->__internal);
}

CE__Iterator CE__strbegin(CE__String* str) {
  return (CE__Iterator) {
    .index = 0,
    .length = str->length,
    .__internal = newITS((struct IteratorSecret) {
      .pointer = str->buffer,
      .step = 0,
      .next = CE__strnext,
      .get = CE__strget
    })
  };
}

bool CE__strnext(CE__Iterator* it) {
  if (it->index >= it->length - 1)
    return false;
  IteratorSecret secret = it->__internal;
  secret->pointer += CE__utf8_char_size(*(secret->pointer));
  it->index++;
  return true;
}

void* CE__strget(CE__Iterator* it) {
  IteratorSecret secret = it->__internal;
  return secret->pointer;
}

CE__Iterator CE__ArrayListBegin(CE__ArrayList* self) {
  return (CE__Iterator) {
    .index = 0,
    .length = self->length,
    .__internal = newITS((struct IteratorSecret) {
      .pointer = self->buffer,
      .step = self->element_size,
      .next = CE__ArrayListNext,
      .get = CE__ArrayListGet
    })
  };
}

bool CE__ArrayListNext(CE__Iterator* it) {
  if (it->index >= it->length - 1)
    return false;
  IteratorSecret secret = it->__internal;
  secret->pointer += secret->step;
  it->index++;
  return true;
}

void* CE__ArrayListGet(CE__Iterator* it) {
  IteratorSecret secret = it->__internal;
  return secret->pointer;
}
