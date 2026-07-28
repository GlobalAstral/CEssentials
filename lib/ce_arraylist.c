#include <CEssentials.h>
#include <CE__Iterator.h>

#define DEFAULT_CAP 16
#define ceil(a, b) ((a + b - 1) / b)
#define getcap(size) (ceil(size, DEFAULT_CAP) * DEFAULT_CAP)

typedef struct ArrayListSecret {
  byte* buffer;
  size_t capacity;
} *ArrayListSecret;

ArrayListSecret newALS(struct ArrayListSecret secret) {
  ArrayListSecret ret = (ArrayListSecret)malloc(sizeof(struct ArrayListSecret));
  *ret = secret;
  return ret;
}

CE__ArrayList CE__newArrayList(size_t element_size) {
  CE__ArrayList ret;
  ret.__internal = newALS((struct ArrayListSecret) {
    .buffer = nullptr
  });
  ret.element_size = element_size;
  ret.length = 0;
  CE__ArrayListRealloc(&ret, DEFAULT_CAP * element_size);
  return ret;
}

void CE__freeArrayList(CE__ArrayList* list) {
  ArrayListSecret secret = list->__internal;
  free(secret->buffer);
  free(secret);
  list->__internal = nullptr;
}

int CE__ArrayListRealloc(CE__ArrayList* list, size_t size) {
  guard(list == nullptr, VALUE_IS_NULL);
  ArrayListSecret secret = list->__internal;
  guard(secret == nullptr, VALUE_IS_FREED);
  size_t needed_size = getcap(size);
  byte* new_buf = (byte*)realloc(secret->buffer, needed_size);
  if (!new_buf)
    return CANNOT_ALLOCATE;
  secret->buffer = new_buf;
  secret->capacity = needed_size;
  return OK;
}

int CE__insertArrayList(CE__ArrayList* self, size_t index, void* item) {
  guard(self == nullptr, VALUE_IS_NULL);
  guard(item == nullptr, OTHER_VALUE_IS_NULL);
  guard(index > self->length, INDEX_OUT_OF_BOUNDS);
  ArrayListSecret secret = self->__internal;
  guard(secret == nullptr, VALUE_IS_FREED);
  
  size_t totalsize = self->length * self->element_size + self->element_size;
  if (secret->capacity < totalsize)
    guard(CE__ArrayListRealloc(self, totalsize), CANNOT_ALLOCATE);
  
  if (totalsize < secret->capacity / 4)
    guard(CE__ArrayListRealloc(self, totalsize), CANNOT_ALLOCATE);
  
  if (index == self->length) {
    memcpy(secret->buffer + self->length * self->element_size, item, self->element_size);
    self->length++;
    return OK;
  }

  memmove( (secret->buffer + index*self->element_size + self->element_size), (secret->buffer + index * self->element_size), (self->length - index)*self->element_size );
  memcpy( (secret->buffer + index * self->element_size), item, self->element_size);
  self->length++;

  return OK;
}

int CE__appendArrayList(CE__ArrayList* self, void* item) {
  return CE__insertArrayList(self, self->length, item);
}

CE__ArrayListView CE__ArrayListSection(CE__ArrayList* self, size_t start, size_t end) {
  ArrayListSecret secret = self->__internal;
  guard(self == nullptr || self->__internal == nullptr || start > self->length || end >= self->length, nullptr);

  CE__ArrayListView view = (CE__ArrayListView)malloc(sizeof(CE__ArrayList));
  *view = (CE__ArrayList) {
    .element_size = self->element_size,
    .length = end - start,
    .__internal = newALS((struct ArrayListSecret) {
      .buffer = secret->buffer + start * self->element_size,
      .capacity = secret->capacity - start * self->element_size,
    })
  };
  return view;
}

void* CE__ArrayListFind(CE__ArrayList* self, void* item) {
  ArrayListSecret secret = self->__internal;
  guard(self == nullptr || item == nullptr || self->__internal == nullptr, nullptr);
  for (size_t i = 0; i < self->length; i++) {
    void* element = secret->buffer + i * self->element_size;
    guard(memcmp(element, item, self->element_size) == 0, element);
  }
  return nullptr;
}

bool CE__ArrayListContains(CE__ArrayList* self, void* item) {
  return CE__ArrayListFind(self, item) != nullptr;
}

int CE__removeArrayList(CE__ArrayList* self, size_t index) {
  ArrayListSecret secret = self->__internal;
  guard(self == nullptr, VALUE_IS_NULL);
  guard(self->__internal == nullptr, VALUE_IS_FREED);
  guard(self->length <= 0, TYPE_EMPTY);
  guard(index >= self->length, INDEX_OUT_OF_BOUNDS);

  if (index == self->length - 1) {
    self->length--;
    return OK;
  }

  memmove(secret->buffer + index * self->element_size, secret->buffer + (index + 1) * self->element_size, (self->length - index - 1) * self->element_size);
  self->length--;
  return OK;
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

CE__Iterator CE__ArrayListBegin(CE__ArrayList* self) {
  ArrayListSecret secret = self->__internal;
  return (CE__Iterator) {
    .index = 0,
    .length = self->length,
    .__internal = newITS((struct IteratorSecret) {
      .pointer = secret->buffer,
      .step = self->element_size,
      .next = CE__ArrayListNext,
      .get = CE__ArrayListGet
    })
  };
}
