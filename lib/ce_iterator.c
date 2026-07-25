#include <CEssentials.h>
#include <CE__Iterator.h>

IteratorSecret newITS(struct IteratorSecret secret) {
  IteratorSecret ret = (IteratorSecret)malloc(sizeof(struct IteratorSecret));
  *ret = secret;
  return ret;
}

bool CE__IteratorNext(CE__Iterator* it) {
  guard(it == nullptr, false);
  guard(it->__internal == nullptr, false);
  IteratorSecret secret = it->__internal;
  return secret->next(it);
}

void* CE__IteratorGet(CE__Iterator* it) {
  guard(it == nullptr, nullptr);
  guard(it->__internal == nullptr, nullptr);
  IteratorSecret secret = it->__internal;
  return secret->get(it);
}

void CE__FreeIterator(CE__Iterator* it) {
  free(it->__internal);
  it->__internal = nullptr;
}
