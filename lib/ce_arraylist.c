#include <CEssentials.h>

#define DEFAULT_CAP 16
#define ceil(a, b) ((a + b - 1) / b)
#define getcap(size) (ceil(size, DEFAULT_CAP) * DEFAULT_CAP)

CE__ArrayList CE__newArrayList(size_t element_size) {
  CE__ArrayList ret;
  ret.buffer = NULL;
  ret.element_size = element_size;
  ret.length = 0;
  CE__ArrayListRealloc(&ret, DEFAULT_CAP * element_size);
  return ret;
}

void CE__freeArrayList(CE__ArrayList* list) {
  free(list->buffer);
  list->isfreed = true;
}

int CE__ArrayListRealloc(CE__ArrayList* list, size_t size) {
  size_t needed_size = getcap(size);
  byte* new_buf = (byte*)realloc(list->buffer, needed_size);
  if (!new_buf)
    return CANNOT_ALLOCATE;
  list->buffer = new_buf;
  list->capacity = needed_size;
  list->isfreed = false;
  return OK;
}

int CE__insertArrayList(CE__ArrayList* self, size_t index, void* item) {
  guard(self == nullptr, VALUE_IS_NULL);
  guard(self->isfreed, VALUE_IS_FREED);
  guard(item == nullptr, OTHER_VALUE_IS_NULL);
  guard(index > self->length, INDEX_OUT_OF_BOUNDS);
  
  size_t totalsize = self->length * self->element_size + self->element_size;
  if (self->capacity < totalsize)
    guard(CE__ArrayListRealloc(self, totalsize), CANNOT_ALLOCATE);
  
  if (totalsize < self->capacity / 4)
    guard(CE__ArrayListRealloc(self, totalsize), CANNOT_ALLOCATE);
  
  if (index == self->length) {
    memcpy(self->buffer + self->length * self->element_size, item, self->element_size);
    self->length++;
    return OK;
  }

  memmove( (self->buffer + index*self->element_size + self->element_size), (self->buffer + index * self->element_size), (self->length - index)*self->element_size );
  memcpy( (self->buffer + index * self->element_size), item, self->element_size);
  self->length++;

  return OK;
}

int CE__appendArrayList(CE__ArrayList* self, void* item) {
  return CE__insertArrayList(self, self->length, item);
}

CE__ArrayListView CE__ArrayListSection(CE__ArrayList* self, size_t start, size_t end) {
  guard(self == nullptr || self->isfreed || start > self->length || end >= self->length, nullptr);

  CE__ArrayListView view = (CE__ArrayListView)malloc(sizeof(CE__ArrayList));
  *view = (CE__ArrayList) {
    .buffer = self->buffer + start * self->element_size,
    .capacity = self->capacity - start * self->element_size,
    .element_size = self->element_size,
    .isfreed = false,
    .length = end - start
  };
  return view;
}

void* CE__ArrayListFind(CE__ArrayList* self, void* item) {
  guard(self == nullptr || item == nullptr || self->isfreed, nullptr);
  for (size_t i = 0; i < self->length; i++) {
    void* element = self->buffer + i * self->element_size;
    guard(memcmp(element, item, self->element_size) == 0, element);
  }
  return nullptr;
}

bool CE__ArrayListContains(CE__ArrayList* self, void* item) {
  return CE__ArrayListFind(self, item) != nullptr;
}

int CE__removeArrayList(CE__ArrayList* self, size_t index) {
  guard(self == nullptr, VALUE_IS_NULL);
  guard(self->isfreed, VALUE_IS_FREED);
  guard(self->length <= 0, TYPE_EMPTY);
  guard(index >= self->length, INDEX_OUT_OF_BOUNDS);

  if (index == self->length - 1) {
    self->length--;
    return OK;
  }

  memmove(self->buffer + index * self->element_size, self->buffer + (index + 1) * self->element_size, (self->length - index - 1) * self->element_size);
  self->length--;
  return OK;
}
