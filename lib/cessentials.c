#include <CEssentials.h>

static const char* const ERRORS[] = {
  "SUCCESS",
  "Value is null",
  "Other value is null",
  "Value is freed",
  "Other value is freed",
  "Cannot Allocate",
  "Index Out of Bounds",
  "Type is Empty",
  "Not Found"
};

const char* const CE__converrno(int err) {
  size_t index = -err;
  if (index >= sizeof(ERRORS) / sizeof(char*))
    return nullptr;
  return ERRORS[index];
}

void* CE__memdup(void* mem, size_t size) {
  void* ret = CE__malloc(size);
  guard(ret == nullptr, nullptr);
  memcpy(ret, mem, size);
  return ret;
}

static CE__Malloc _m = malloc;
static CE__Realloc _r = realloc;
static CE__Calloc _c = calloc;
static CE__Free _f = free;

void CE__selectAllocators(CE__Malloc m, CE__Realloc r, CE__Calloc c, CE__Free f) {
  _m = m;
  _r = r;
  _c = c;
  _f = f;
}

void* CE__malloc(size_t size) {
  return _m(size);
}

void* CE__realloc(void* buf, size_t size) {
  return _r(buf, size);
}

void* CE__calloc(size_t size, size_t ele_size) {
  return _c(size, ele_size);
}

void CE__free(void* buf) {
  _f(buf);
}
