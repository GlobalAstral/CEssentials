#include <CEssentials.h>

struct CE__BitArray {
  byte* bytes;
  size_t length;
};

CE__BitArray CE__newBitArray(size_t size) {
  CE__BitArray ret = (CE__BitArray)malloc(sizeof(*ret));
  ret->length = size;
  ret->bytes = (byte*)malloc(ret->length);
  return ret;
}

void CE__freeBitArray(CE__BitArray self) {
  free(self->bytes);
  free(self);
}

bool CE__getBitArray(CE__BitArray self, size_t bit) {
  guard(bit >= self->length * 8, false);
  size_t i = bit / 8;
  size_t b = bit % 8;
  return (self->bytes[i] >> b) & 1;
}

void CE__setBitArray(CE__BitArray self, size_t bit, bool flag) {
  if (bit >= self->length * 8)
    return;
  size_t i = bit / 8;
  size_t b = bit % 8;
  if (flag)
    self->bytes[i] |= (1ULL << b);
  else
    self->bytes[i] &= ~(1ULL << b);
}

void CE__clearBitArray(CE__BitArray self) {
  memset(self->bytes, 0, self->length);
}

void CE__toggleBitArray(CE__BitArray self, size_t bit) {
  if (bit >= self->length * 8)
    return;
  size_t i = bit / 8;
  size_t b = bit % 8;
  self->bytes[i] ^= (1ULL << b);
}
