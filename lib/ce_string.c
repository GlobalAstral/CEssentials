#include <CEssentials.h>
#include <CE__Iterator.h>

#define DEFAULT_CAP 64
#define ceil(a, b) ((a + b - 1) / b)
#define getcap(size) (ceil(size, DEFAULT_CAP) * DEFAULT_CAP)

struct CE__String {
  size_t bytelen;
  size_t length;
  byte* buffer;
  size_t capacity;
};

size_t CE__utf8len(CE__String s) {
  guard(s->buffer == nullptr, VALUE_IS_FREED);

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
  CE__String ret = (CE__String)malloc(sizeof(*ret));
  if (!init) {
    ret->bytelen = 0;
    ret->buffer = nullptr;
    CE__strRealloc(ret, size);
    ret->length = 0;
    return ret;
  }

  ret->bytelen = size;
  ret->buffer = nullptr;
  CE__strRealloc(ret, ret->bytelen);
  memcpy(ret->buffer, init, ret->bytelen);
  ret->length = CE__utf8len(ret);
  return ret;
}

void CE__freeString(CE__String s) {
  free(s->buffer);
  free(s);
}

int CE__strRealloc(CE__String s, size_t size) {
  size_t needed_size = getcap(size);
  byte* new_buf = (byte*)realloc(s->buffer, needed_size);

  guard(!new_buf, CANNOT_ALLOCATE);
  
  s->buffer = new_buf;
  s->capacity = needed_size;
  return OK;
}

size_t CE__utf8_byte_index(CE__String target, size_t index) {
  guard(target == nullptr, VALUE_IS_NULL);
  guard(target->buffer == nullptr, VALUE_IS_FREED);
  guard(index == 0, 0);
  guard(index == target->length, target->bytelen);

  size_t codepoint = 0;

  for (size_t i = 0; i < target->bytelen; i++) {
    if ((target->buffer[i] & 0xC0) != 0x80) {
      guard(codepoint == index, i);
      codepoint++;
    }
  }

  return target->bytelen;
}

int CE__insertString(CE__String self, size_t index, CE__String other) {
  guard(self == nullptr, VALUE_IS_NULL);
  guard(self->buffer == nullptr, VALUE_IS_FREED);
  guard(other == nullptr, OTHER_VALUE_IS_NULL);
  guard(other->buffer == nullptr, OTHER_VALUE_IS_FREED);
  guard(index > self->length, INDEX_OUT_OF_BOUNDS);
  size_t totalsize = self->bytelen + other->bytelen;
  if (self->capacity < totalsize)
    guard(CE__strRealloc(self, totalsize), CANNOT_ALLOCATE);

  if (totalsize < self->capacity / 4) {
    guard(CE__strRealloc(self, totalsize), CANNOT_ALLOCATE);
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
int CE__insertCstr(CE__String self, size_t index, char* s) {
  guard(self == nullptr, VALUE_IS_NULL);
  guard(self->buffer == nullptr, VALUE_IS_FREED);
  guard(!s, OTHER_VALUE_IS_NULL);

  CE__String temp = CE__newString(s);
  int r = CE__insertString(self, index, temp);
  CE__freeString(temp);
  return r;
}
int CE__appendString(CE__String self, CE__String other) {
  return CE__insertString(self, self->length, other);
}
int CE__appendCstr(CE__String self, char* other) {
  return CE__insertCstr(self, self->length, other);
}

int CE__strcmp(CE__String self, CE__String other) {
  size_t n = self->bytelen < other->bytelen ? self->bytelen : other->bytelen;
  int result = memcmp(self->buffer, other->buffer, n);

  if (result < 0) return -1;
  if (result > 0) return 1;
  if (self->bytelen < other->bytelen) return -1;
  if (self->bytelen > other->bytelen) return 1;
  return 0;
}

bool CE__strequ(CE__String self, CE__String other) {
  return CE__strcmp(self, other) == 0;
}

char* CE__strcstr(CE__String self) {
  guard(self == nullptr, nullptr);
  guard(self->buffer == nullptr, nullptr);

  char* temp = (char*)malloc(self->bytelen+1);
  memcpy(temp, self->buffer, self->bytelen);
  temp[self->bytelen] = 0;
  return temp;
}

CE__String CE__substr(CE__String self, size_t start, size_t end) {
  guard(self == nullptr || self->buffer == nullptr || start > self->length || end > self->length, nullptr);

  size_t bstart = CE__utf8_byte_index(self, start);
  size_t bend = CE__utf8_byte_index(self, end);
  CE__String r = (CE__String)malloc(sizeof(struct CE__String));
  *r = (struct CE__String) {
    .length = end - start,
    .bytelen = bend - bstart,
    .buffer = self->buffer + bstart,
    .capacity = self->capacity - bstart
  };
  return r;
}

CE__String CE__strfind(CE__String self, CE__String find) {
  guard(self == nullptr || find == nullptr || self->buffer == nullptr || find->buffer == nullptr, nullptr);

  for (size_t i = 0; i < self->length - find->length; i++) {
    CE__String sub = CE__substr(self, i, i + find->length);
    guard(CE__strequ(sub, find), sub);
  }
  return nullptr;
}

bool CE__strcontains(CE__String self, CE__String find) {
  CE__String r = CE__strfind(self, find);
  bool flag = r != nullptr;
  free(r);
  return flag;
}

int CE__printstr(CE__String self) {
  char* temp = CE__strcstr(self);
  int r = printf("%s", temp);
  free(temp);
  return r;
}

int CE__fprintstr(FILE* stream, CE__String self) {
  char* temp = CE__strcstr(self);
  int r = fprintf(stream, "%s", temp);
  free(temp);
  return r;
}

int CE__strdrain(CE__String self, size_t start, size_t end) {
  guard(self == nullptr, VALUE_IS_NULL);
  guard(self->buffer == nullptr, VALUE_IS_FREED);
  guard(start > self->length || end > self->length, INDEX_OUT_OF_BOUNDS);
  
  size_t bstart = CE__utf8_byte_index(self, start);
  size_t bend = CE__utf8_byte_index(self, end);
  if (end == self->length) {
    self->length -= end - start;
    self->bytelen -= bend - bstart;
    return OK;
  }

  memmove(self->buffer + bstart, self->buffer + bend, self->bytelen - bend + 1);
  self->length -= end - start;
  self->bytelen -= bend - bstart;

  return OK;
}

size_t CE__strlength(CE__String s) {
  return s->length;
}
size_t CE__strblen(CE__String s) {
  return s->bytelen;
}

size_t CE__utf8_char_size(byte b) {
  if ((b & 0x80) == 0) return 1;
  if ((b & 0xE0) == 0xC0) return 2;
  if ((b & 0xF0) == 0xE0) return 3;
  if ((b & 0xF8) == 0xF0) return 4;
  return 0;
}

bool CE__strnext(CE__Iterator it) {
  if (it->index >= it->length - 1)
    return false;
  it->pointer += CE__utf8_char_size(*(it->pointer));
  it->index++;
  return true;
}

void* CE__strget(CE__Iterator it) {
  return it->pointer;
}

CE__Iterator CE__strbegin(CE__String str) {
  CE__Iterator ret = (CE__Iterator)malloc(sizeof(*ret));
  *ret = (struct CE__Iterator) {
    .index = 0,
    .length = str->length,
    .pointer = str->buffer,
    .step = 0,
    .next = CE__strnext,
    .get = CE__strget
  };
  return ret;
}
