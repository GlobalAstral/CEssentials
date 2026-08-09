#include <CEssentials.h>
#include <xxhash.h>

#include <CE__Iterator.h>

struct CE__Hash128 {
  unsigned long long low;
  unsigned long long high;
};

CE__Hash128 CE__hash128(void* data, size_t size) {
  XXH128_hash_t h = XXH3_128bits(data, size);
  CE__Hash128 ret = (CE__Hash128)CE__malloc(sizeof(*ret));
  guard(!ret, nullptr);
  ret->high = h.high64;
  ret->low = h.low64;
  return ret;
}

void CE__freeHash128(CE__Hash128 self) {
  CE__free(self);
}

bool CE__Hash128Equals(CE__Hash128 a, CE__Hash128 b) {
  guard(a == nullptr && b == nullptr, true);
  guard(a == nullptr, false);
  guard(b == nullptr, false);
  return a->high == b->high && a->low == b->low;
}

unsigned long long CE__Hash128Modulo(CE__Hash128 self, unsigned long long mod) {
  return XXH3_64bits(self, sizeof(*self)) % mod;
}
