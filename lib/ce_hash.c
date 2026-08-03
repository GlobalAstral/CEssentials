#include <CEssentials.h>

#include <xxhash.h>

struct CE__Hash128 {
  unsigned long long low;
  unsigned long long high;
};

CE__Hash128 CE__hash128(void* data, size_t size) {
  XXH128_hash_t h = XXH3_128bits(data, size);
  CE__Hash128 ret = (CE__Hash128)malloc(sizeof(*ret));
  ret->high = h.high64;
  ret->low = h.low64;
  return ret;
}

void CE__freeHash128(CE__Hash128 self) {
  free(self);
}

bool CE__Hash128Equals(CE__Hash128 a, CE__Hash128 b) {
  guard(a == nullptr && b == nullptr, true);
  guard(a == nullptr, false);
  guard(b == nullptr, false);
  return a->high == b->high && a->low == b->low;
}
