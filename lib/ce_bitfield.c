#include <CEssentials.h>

struct CE__BitField {
  unsigned long long flags;
  unsigned long long initial;
};

CE__BitField CE__newBitFieldEx(unsigned long long init) {
  CE__BitField ret = (CE__BitField)malloc(sizeof(*ret));
  guard(!ret, nullptr);
  ret->flags = init;
  ret->initial = init;
  return ret;
}

CE__BitField CE__newBitField() {
  return CE__newBitFieldEx(0);
}

void CE__freeBitField(CE__BitField self) {
  free(self);
}

bool CE__getBitField(CE__BitField self, unsigned char bit) {  
  return (self->flags >> bit) & 1;
}

void CE__setBitField(CE__BitField self, unsigned char bit, bool flag) {
  if (flag)
    self->flags |= (1ULL << bit);
  else
    self->flags &= ~(1ULL << bit);
}

void CE__clearBitField(CE__BitField self) {
  self->flags = self->initial;
}

void CE__toggleBitField(CE__BitField self, unsigned char bit) {
  self->flags ^= (1ULL << bit);
}
