#include <CEssentials.h>

#define DEFAULT_CAP 128
#define ceil(a, b) ((a + b - 1) / b)
#define getcap(size) (ceil(size, DEFAULT_CAP) * DEFAULT_CAP)

size_t CE_utf8len(CE__String* s) {
  size_t count = 0;
  for (size_t i = 0; i < s->bytelen; i++) {
    byte c = s->buffer[i];
    if ((c & 0xC0) != 0x80)
      count++;
  }
  return count;
}

CE__String CE_newString(char* init) {
  CE__String ret;
  if (!init) {
    ret.buffer = (byte*)malloc(DEFAULT_CAP);
    ret.capacity = DEFAULT_CAP;
    ret.length = 0;
    ret.bytelen = 0;
    return ret;
  }

  ret.bytelen = strlen(init);
  ret.capacity = getcap(ret.bytelen);
  ret.buffer = (byte*)malloc(ret.bytelen + 1);
  memcpy(ret.buffer, init, ret.bytelen);
  ret.length = CE_utf8len(&ret);
  return ret;
}

void CE_freeString(CE__String* s) {
  free(s->buffer);
}

int CE_strRealloc(CE__String* s, size_t size) {
  size_t needed_size = getcap(size);
  byte* new_buf = (byte*)realloc(s->buffer, needed_size);
  if (!new_buf)
    return 1;
  s->buffer = new_buf;
  s->capacity = needed_size;
  return 0;
}

size_t CE_utf8_byte_index(CE__String* target, size_t index) {
  if (index == 0)
    return 0;
  if (index == target->length)
    return target->bytelen;

  size_t codepoint = 0;

  for (size_t i = 0; i < target->bytelen; i++) {
    if ((target->buffer[i] & 0xC0) != 0x80) {
      if (codepoint == index)
        return i;
      codepoint++;
    }
  }

  return target->bytelen;
}

int CE_insertString(CE__String* self, size_t index, CE__String* other) {
  if (index < 0 || index > self->length)
    return -1;
  size_t totalsize = self->bytelen + other->bytelen;
  if (self->capacity < totalsize) {
    if (CE_strRealloc(self, totalsize))
      return 1;
  }

  size_t byteindex = CE_utf8_byte_index(self, index);

  memmove( (self->buffer + byteindex + other->bytelen), (self->buffer + byteindex), (self->bytelen - byteindex + 1) );
  memcpy( (self->buffer + byteindex), other->buffer, other->bytelen);

  self->length += other->length;
  self->bytelen += other->bytelen;

  return 0;
}
