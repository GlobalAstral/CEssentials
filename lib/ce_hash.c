#include <CEssentials.h>
#include <xxhash.h>

#include <CE__Iterator.h>

#define DEFAULT_CAPACITY 64
#define ceil(a, b) ((a + b - 1) / b)
#define getcap(size) ( ((size) == 0) ? (DEFAULT_CAPACITY) : (ceil(size, DEFAULT_CAPACITY) * DEFAULT_CAPACITY) )

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

typedef struct Bucket {
  CE__Hash128 hash;
  void* key;
  void* value;
  struct Bucket* next;
} *Bucket;

struct CE__HashMap {
  size_t length;
  size_t capacity;
  size_t key_size;
  size_t value_size;
  Bucket* buckets;

  CE__HashMapEquals equals;
};

size_t CE__lengthHashMap(CE__HashMap self) {
  return self->length;
}

CE__HashMap CE__newHashMapEx(size_t key_size, size_t value_size, CE__HashMapEquals equals) {
  CE__HashMap ret = (CE__HashMap)CE__malloc(sizeof(*ret));
  guard(!ret, nullptr);
  
  ret->capacity = DEFAULT_CAPACITY;
  ret->equals = equals;
  ret->key_size = key_size;
  ret->length = 0;
  ret->value_size = value_size;
  ret->buckets = CE__calloc(ret->capacity, sizeof(Bucket));
  guard(ret->buckets == nullptr, nullptr);

  return ret;
}

bool default_equals(void* a, void* b, size_t size) {
  return memcmp(a, b, size) == 0;
}

CE__HashMap CE__newHashMap(size_t key_size, size_t value_size) {
  return CE__newHashMapEx(key_size, value_size, default_equals);
}

void CE__freeHashMap(CE__HashMap self) {
  if (self == nullptr)
    return;
  for (size_t i = 0; i < self->capacity; i++) {
    Bucket b = self->buckets[i];
    if (b == nullptr) continue;
    while (b != nullptr) {
      Bucket next = b->next;
      CE__free(b->key);
      CE__free(b->value);
      CE__freeHash128(b->hash);
      CE__free(b);
      b = next;
    }
  }
  CE__free(self->buckets);
  CE__free(self);
}

int resize(CE__HashMap self, size_t newsize) {
  guard(self == nullptr, VALUE_IS_NULL);

  if (self->capacity == newsize)
    return OK;

  Bucket* new_buckets = CE__calloc(newsize, sizeof(Bucket));
  guard(new_buckets == nullptr, CANNOT_ALLOCATE);

  for (size_t i = 0; i < self->capacity; i++) {
    Bucket b = self->buckets[i];

    while (b != nullptr) {
      Bucket next = b->next;

      size_t index = CE__Hash128Modulo(b->hash, newsize);

      b->next = new_buckets[index];
      new_buckets[index] = b;

      b = next;
    }
  }

  self->capacity = newsize;

  CE__free(self->buckets);
  self->buckets = new_buckets;

  return OK;
}

Bucket newBucket(CE__Hash128 hash, void* key, void* value, Bucket next) {
  Bucket ret = (Bucket)CE__malloc(sizeof(*ret));
  guard(!ret, nullptr);

  *ret = (struct Bucket) {
    .hash = hash,
    .key = key,
    .value = value,
    .next = next
  };

  return ret;
}

int CE__insertHashMap(CE__HashMap self, void* key, void* value) {
  guard(self == nullptr, VALUE_IS_NULL);
  guard(key == nullptr, OTHER_VALUE_IS_NULL);
  guard(value == nullptr, OTHER_VALUE_IS_NULL);
  
  CE__Hash128 hash = CE__hash128(key, self->key_size);
  size_t index = CE__Hash128Modulo(hash, self->capacity);

  Bucket* bucket = &self->buckets[index];

  loop {
    if (*bucket == nullptr) {
      void* k = CE__memdup(key, self->key_size);
      guard(k == nullptr, CANNOT_ALLOCATE);
      
      void* v = CE__memdup(value, self->value_size);
      if (v == nullptr) {
        CE__free(k);
        return CANNOT_ALLOCATE;
      }

      *bucket = newBucket(hash, k, v, nullptr);
      if (*bucket == nullptr) {
        CE__free(k);
        CE__free(v);
        return CANNOT_ALLOCATE;
      }

      self->length++;
      return OK;
    }

    if (CE__Hash128Equals((*bucket)->hash, hash) && self->equals(key, (*bucket)->key, self->key_size)) {
      memcpy((*bucket)->value, value, self->value_size);
      return OK;
    }

    bucket = &((*bucket)->next);
  }

  if (((double)self->length  * sizeof(Bucket) / self->capacity) > 0.7) {
    size_t newsize = self->capacity * 2;
    guard(resize(self, newsize) != OK, CANNOT_ALLOCATE);
  }

  return OK;
}

void* CE__getHashMap(CE__HashMap self, void* key) {
  guard(self == nullptr, nullptr);
  guard(key == nullptr, nullptr);

  CE__Hash128 hash = CE__hash128(key, self->key_size);
  size_t index = CE__Hash128Modulo(hash, self->capacity);

  Bucket* bucket = &self->buckets[index];

  loop {
    if (*bucket == nullptr)
      return nullptr;

    if (CE__Hash128Equals((*bucket)->hash, hash) && self->equals(key, (*bucket)->key, self->key_size))
      return (*bucket)->value;

    bucket = &((*bucket)->next);
  }
  return nullptr;
}

bool CE__containsHashMap(CE__HashMap self, void* key) {
  return CE__getHashMap(self, key) != nullptr;
}

int CE__removeHashMap(CE__HashMap self, void* key) {
  guard(self == nullptr, VALUE_IS_NULL);
  guard(key == nullptr, OTHER_VALUE_IS_NULL);
  
  CE__Hash128 hash = CE__hash128(key, self->key_size);
  size_t index = CE__Hash128Modulo(hash, self->capacity);

  Bucket* bucket = &self->buckets[index];

  loop {
    if (*bucket == nullptr) {
      CE__freeHash128(hash);
      return NOT_FOUND;
    }
    Bucket b = *bucket;

    if (CE__Hash128Equals(b->hash, hash) && self->equals(key, b->key, self->key_size)) {
      *bucket = b->next;
      CE__freeHash128(b->hash);
      CE__freeHash128(hash);
      CE__free(b->key);
      CE__free(b->value);
      CE__free(b);
      self->length--;

      if (((double)self->length * sizeof(Bucket) / self->capacity) <= 0.33) {
        size_t newsize = getcap(self->length);
        if (newsize != self->capacity)
          guard(resize(self, newsize) != OK, CANNOT_ALLOCATE);
      }

      return OK;
    }

    bucket = &(b->next);
  }

  CE__freeHash128(hash);
  return OK;
}

int CE__reserveHashMap(CE__HashMap self, size_t amount) {
  size_t newsize = getcap(amount);
  return resize(self, newsize);
}

void* CE__getOrCreateHashMap(CE__HashMap self, void* key, void* value) {
  void* v = CE__getHashMap(self, key);
  if (v != nullptr)
    return v;
  CE__insertHashMap(self, key, value);
  return CE__getHashMap(self, key);
}
