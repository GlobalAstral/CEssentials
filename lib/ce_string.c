#include <CEssentials.h>

#define DEFAULT_CAP 128
#define ceil(a, b) ((a + b - 1) / b)
#define getcap(size) (ceil(size, DEFAULT_CAP) * DEFAULT_CAP)

size_t CE__utf8len(CE__String* s) {
  if (s->isfreed)
    return VALUE_IS_FREED;
  size_t count = 0;
  for (size_t i = 0; i < s->bytelen; i++) {
    byte c = s->buffer[i];
    if ((c & 0xC0) != 0x80)
      count++;
  }
  return count;
}

CE__String CE__newString(char* init) {
  size_t sz = init ? strlen(init) : 0;
  return CE__newStringLen(
    init, 
    sz
  );
}

CE__String CE__newStringLen(char* init, size_t size) {
  CE__String ret;
  if (!init) {
    ret.bytelen = 0;
    ret.buffer = NULL;
    CE__strRealloc(&ret, size);
    ret.length = 0;
    return ret;
  }

  ret.bytelen = size;
  ret.buffer = NULL;
  CE__strRealloc(&ret, ret.bytelen);
  memcpy(ret.buffer, init, ret.bytelen);
  ret.length = CE__utf8len(&ret);
  return ret;
}

void CE__freeString(CE__String* s) {
  free(s->buffer);
  s->isfreed = true;
}

int CE__strRealloc(CE__String* s, size_t size) {
  size_t needed_size = getcap(size);
  byte* new_buf = (byte*)realloc(s->buffer, needed_size);
  if (!new_buf)
    return CANNOT_ALLOCATE;
  s->buffer = new_buf;
  s->capacity = needed_size;
  s->isfreed = false;
  return OK;
}

size_t CE__utf8_byte_index(CE__String* target, size_t index) {
  if (target == NULL)
    return VALUE_IS_NULL;
  if (target->isfreed)
    return VALUE_IS_FREED;

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

int CE__insertString(CE__String* self, size_t index, CE__String* other) {
  if (self == NULL)
    return VALUE_IS_NULL;
  if (other == NULL)
    return OTHER_VALUE_IS_NULL;
  if (self->isfreed)
    return VALUE_IS_FREED;
  if (other->isfreed)
    return OTHER_VALUE_IS_FREED;

  if (index < 0 || index > self->length)
    return INDEX_OUT_OF_BOUNDS;
  size_t totalsize = self->bytelen + other->bytelen;
  if (self->capacity < totalsize) {
    if (CE__strRealloc(self, totalsize))
      return CANNOT_ALLOCATE;
  }

  if (index == self->length) {
    memcpy(self->buffer + self->bytelen, other->buffer, other->bytelen);
    self->length += other->length;
    self->bytelen += other->bytelen;
    return OK;
  }

  size_t byteindex = CE__utf8_byte_index(self, index);

  memmove( (self->buffer + byteindex + other->bytelen), (self->buffer + byteindex), (self->bytelen - byteindex + 1) );
  memcpy( (self->buffer + byteindex), other->buffer, other->bytelen);

  self->length += other->length;
  self->bytelen += other->bytelen;

  return OK;
}
int CE__insertCstr(CE__String* self, size_t index, char* s) {
  if (self == NULL)
    return VALUE_IS_NULL;
  if (self->isfreed)
    return VALUE_IS_FREED;
  if (!s)
    return OTHER_VALUE_IS_NULL;
  CE__String temp = CE__newString(s);
  int r = CE__insertString(self, index, &temp);
  CE__freeString(&temp);
  return r;
}
int CE__appendString(CE__String* self, CE__String* other) {
  return CE__insertString(self, self->length, other);
}
int CE__appendCstr(CE__String* self, char* other) {
  return CE__insertCstr(self, self->length, other);
}

int CE__strcmp(CE__String* self, CE__String* other) {
  size_t n = self->bytelen < other->bytelen ? self->bytelen : other->bytelen;
  int result = memcmp(self->buffer, other->buffer, n);

  if (result < 0) return -1;
  if (result > 0) return 1;
  if (self->bytelen < other->bytelen) return -1;
  if (self->bytelen > other->bytelen) return 1;
  return 0;
}

bool CE__strequ(CE__String* self, CE__String* other) {
  return CE__strcmp(self, other) == 0;
}

char* CE__strcstr(CE__String* self) {
  if (self == NULL)
    return VALUE_IS_NULL;
  if (self->isfreed)
    return VALUE_IS_FREED;
  char* temp = (char*)malloc(self->bytelen+1);
  memcpy(temp, self->buffer, self->bytelen);
  temp[self->bytelen] = 0;
  return temp;
}

CE__StrView CE__substr(CE__String* self, size_t start, size_t end) {
  if (self == NULL)
    return NULL;
  if (self->isfreed)
    return NULL;
  if (start < 0 || start >= self->length || end < 0 || end >= self->length)
    return NULL;
  size_t bstart = CE__utf8_byte_index(self, start);
  size_t bend = CE__utf8_byte_index(self, end);
  CE__StrView r = (CE__StrView)malloc(sizeof(CE__String));
  *r = (CE__String) {
    .buffer = self->buffer + bstart,
    .capacity = self->capacity - bstart,
    .length = end - start,
    .bytelen = bend - bstart
  };
  return r;
}

CE__StrView CE__strfind(CE__String* self, CE__String* find) {
  if (self == NULL)
    return NULL;
  if (find == NULL)
    return NULL;
  if (self->isfreed)
    return NULL;
  if (find->isfreed)
    return OTHER_VALUE_IS_FREED;
  for (size_t i = 0; i < self->length - find->length; i++) {
    CE__StrView sub = CE__substr(self, i, i + find->length);
    if (CE__strequ(sub, find))
      return sub;
  }
  return NULL;
}

bool CE__strcontains(CE__String* self, CE__String* find) {
  CE__String* r = CE__strfind(self, find);
  bool flag = r != NULL;
  free(r);
  return flag;
}

int CE__printstr(CE__String* self) {
  char* temp = CE__strcstr(self);
  int r = printf("%s", temp);
  free(temp);
  return r;
}

int CE__fprintstr(FILE* stream, CE__String* self) {
  char* temp = CE__strcstr(self);
  int r = fprintf(stream, "%s", temp);
  free(temp);
  return r;
}
