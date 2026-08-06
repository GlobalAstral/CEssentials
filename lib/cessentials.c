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
  void* ret = malloc(size);
  guard(ret == nullptr, nullptr);
  memcpy(ret, mem, size);
  return ret;
}
