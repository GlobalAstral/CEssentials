#include <CEssentials.h>
#include <CE__Iterator.h>

size_t CE__IteratorIndex(CE__Iterator it) {
  return it->index;
}

size_t CE__IteratorLength(CE__Iterator it) {
  return it->length;
}

bool CE__IteratorNext(CE__Iterator it) {
  guard(it == nullptr, false);
  guard(it->pointer == nullptr, false);
  return it->next(it);
}

void* CE__IteratorGet(CE__Iterator it) {
  guard(it == nullptr, nullptr);
  guard(it->pointer == nullptr, nullptr);
  return it->get(it);
}

void CE__FreeIterator(CE__Iterator it) {
  free(it);
}
