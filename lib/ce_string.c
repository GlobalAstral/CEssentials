#include <CEssentials.h>
#include <CE__Iterator.h>

#define DEFAULT_CAP 64
#define ceil(a, b) ((a + b - 1) / b)
#define getcap(size) (ceil(size, DEFAULT_CAP) * DEFAULT_CAP)

typedef struct StringSecret {
  byte* buffer;
  size_t capacity;
} *StringSecret;

StringSecret newSS(struct StringSecret secret) {
  StringSecret ret = (StringSecret)malloc(sizeof(struct StringSecret));
  *ret = secret;
  return ret;
}

size_t CE__utf8len(CE__String* s) {
  StringSecret secret = s->__internal;
  guard(secret == nullptr, VALUE_IS_FREED);

  size_t count = 0;
  for (size_t i = 0; i < s->bytelen; i++) {
    byte c = secret->buffer[i];
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
    ret.__internal = newSS((struct StringSecret) {
      .buffer = nullptr
    });
    CE__strRealloc(&ret, size);
    ret.length = 0;
    return ret;
  }

  ret.bytelen = size;
  ret.__internal = newSS((struct StringSecret) {
    .buffer = nullptr
  });
  StringSecret secret = ret.__internal;
  CE__strRealloc(&ret, ret.bytelen);
  memcpy(secret->buffer, init, ret.bytelen);
  ret.length = CE__utf8len(&ret);
  return ret;
}

void CE__freeString(CE__String* s) {
  StringSecret secret = s->__internal;
  free(secret->buffer);
  free(secret);
}

int CE__strRealloc(CE__String* s, size_t size) {
  StringSecret secret = s->__internal;
  size_t needed_size = getcap(size);
  byte* new_buf = (byte*)realloc(secret->buffer, needed_size);

  guard(!new_buf, CANNOT_ALLOCATE);
  
  secret->buffer = new_buf;
  secret->capacity = needed_size;
  return OK;
}

size_t CE__utf8_byte_index(CE__String* target, size_t index) {
  StringSecret secret = target->__internal;
  guard(target == nullptr, VALUE_IS_NULL);
  guard(secret == nullptr, VALUE_IS_FREED);
  guard(index == 0, 0);
  guard(index == target->length, target->bytelen);

  size_t codepoint = 0;

  for (size_t i = 0; i < target->bytelen; i++) {
    if ((secret->buffer[i] & 0xC0) != 0x80) {
      guard(codepoint == index, i);
      codepoint++;
    }
  }

  return target->bytelen;
}

int CE__insertString(CE__String* self, size_t index, CE__String* other) {
  StringSecret secret = self->__internal;
  StringSecret osecret = other->__internal;
  guard(self == nullptr, VALUE_IS_NULL);
  guard(secret == nullptr, VALUE_IS_FREED);
  guard(other == nullptr, OTHER_VALUE_IS_NULL);
  guard(osecret == nullptr, OTHER_VALUE_IS_FREED);
  guard(index > self->length, INDEX_OUT_OF_BOUNDS);
  size_t totalsize = self->bytelen + other->bytelen;
  if (secret->capacity < totalsize)
    guard(CE__strRealloc(self, totalsize), CANNOT_ALLOCATE);

  if (totalsize < secret->capacity / 4) {
    guard(CE__strRealloc(self, totalsize), CANNOT_ALLOCATE);
  }

  if (index == self->length) {
    memcpy(secret->buffer + self->bytelen, osecret->buffer, other->bytelen);
    self->length += other->length;
    self->bytelen += other->bytelen;
    return OK;
  }

  size_t byteindex = CE__utf8_byte_index(self, index);

  memmove( (secret->buffer + byteindex + other->bytelen), (secret->buffer + byteindex), (self->bytelen - byteindex + 1) );
  memcpy( (secret->buffer + byteindex), osecret->buffer, other->bytelen);

  self->length += other->length;
  self->bytelen += other->bytelen;

  return OK;
}
int CE__insertCstr(CE__String* self, size_t index, char* s) {
  StringSecret secret = self->__internal;
  guard(self == nullptr, VALUE_IS_NULL);
  guard(secret == nullptr, VALUE_IS_FREED);
  guard(!s, OTHER_VALUE_IS_NULL);

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
  StringSecret secret = self->__internal;
  StringSecret osecret = other->__internal;
  size_t n = self->bytelen < other->bytelen ? self->bytelen : other->bytelen;
  int result = memcmp(secret->buffer, osecret->buffer, n);

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
  StringSecret secret = self->__internal;
  guard(self == nullptr, nullptr);
  guard(secret == nullptr, nullptr);

  char* temp = (char*)malloc(self->bytelen+1);
  memcpy(temp, secret->buffer, self->bytelen);
  temp[self->bytelen] = 0;
  return temp;
}

CE__StrView CE__substr(CE__String* self, size_t start, size_t end) {
  StringSecret secret = self->__internal;
  guard(self == nullptr || secret == nullptr || start > self->length || end > self->length, nullptr);

  size_t bstart = CE__utf8_byte_index(self, start);
  size_t bend = CE__utf8_byte_index(self, end);
  CE__StrView r = (CE__StrView)malloc(sizeof(CE__String));
  *r = (CE__String) {
    .length = end - start,
    .bytelen = bend - bstart,
    .__internal = newSS((struct StringSecret) {
      .buffer = secret->buffer + bstart,
      .capacity = secret->capacity - bstart,
    })
  };
  return r;
}

CE__StrView CE__strfind(CE__String* self, CE__String* find) {
  StringSecret secret = self->__internal;
  StringSecret fsecret = find->__internal;
  guard(self == nullptr || find == nullptr || secret == nullptr || fsecret == nullptr, nullptr);

  for (size_t i = 0; i < self->length - find->length; i++) {
    CE__StrView sub = CE__substr(self, i, i + find->length);
    guard(CE__strequ(sub, find), sub);
  }
  return nullptr;
}

bool CE__strcontains(CE__String* self, CE__String* find) {
  CE__String* r = CE__strfind(self, find);
  bool flag = r != nullptr;
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

int CE__strdrain(CE__String* self, size_t start, size_t end) {
  StringSecret secret = self->__internal;
  guard(self == nullptr, VALUE_IS_NULL);
  guard(secret == nullptr, VALUE_IS_FREED);
  guard(start > self->length || end > self->length, INDEX_OUT_OF_BOUNDS);
  
  size_t bstart = CE__utf8_byte_index(self, start);
  size_t bend = CE__utf8_byte_index(self, end);
  if (end == self->length) {
    self->length -= end - start;
    self->bytelen -= bend - bstart;
    return OK;
  }

  memmove(secret->buffer + bstart, secret->buffer + bend, self->bytelen - bend + 1);
  self->length -= end - start;
  self->bytelen -= bend - bstart;

  return OK;
}

size_t CE__utf8_char_size(byte b) {
  if ((b & 0x80) == 0) return 1;
  if ((b & 0xE0) == 0xC0) return 2;
  if ((b & 0xF0) == 0xE0) return 3;
  if ((b & 0xF8) == 0xF0) return 4;
  return 0;
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

CE__Iterator CE__strbegin(CE__String* str) {
  StringSecret secret = str->__internal;
  return (CE__Iterator) {
    .index = 0,
    .length = str->length,
    .__internal = newITS((struct IteratorSecret) {
      .pointer = secret->buffer,
      .step = 0,
      .next = CE__strnext,
      .get = CE__strget
    })
  };
}
