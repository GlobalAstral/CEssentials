#include <CEssentials.h>

#define DEFAULT_CAPACITY 64
#define ceil(a, b) ((a + b - 1) / b)
#define getcap(size) ( ((size) == 0) ? (DEFAULT_CAPACITY) : (ceil(size, DEFAULT_CAPACITY) * DEFAULT_CAPACITY) )

typedef struct Bucket {
  void* value;
  CE__Hash128 hash;
  struct Bucket* next;
} *Bucket;

struct CE__HashSet {
  size_t length;
  size_t capacity;
  size_t element_size;
  Bucket* buckets;

  CE__HashSetEquals equals;
};

size_t CE__lengthHashSet(CE__HashSet set) {
  return set->length;
}

CE__HashSet CE__newHashSetEx(size_t element_size, CE__HashSetEquals equals) {
  CE__HashSet ret = (CE__HashSet)CE__malloc(sizeof(*ret));
  guard(!ret, nullptr);
  
  ret->capacity = DEFAULT_CAPACITY;
  ret->equals = equals;
  ret->length = 0;
  ret->element_size = element_size;
  ret->buckets = CE__calloc(ret->capacity, sizeof(Bucket));
  guard(ret->buckets == nullptr, nullptr);

  return ret;
}

bool default_equals_hashset(void* a, void* b, size_t size) {
  return memcmp(a, b, size) == 0;
}

CE__HashSet CE__newHashSet(size_t element_size) {
  return CE__newHashSetEx(element_size, default_equals_hashset);
}

void CE__freeHashSet(CE__HashSet set) {
  if (set == nullptr)
    return;
  for (size_t i = 0; i < set->capacity; i++) {
    Bucket b = set->buckets[i];
    if (b == nullptr) continue;
    while (b != nullptr) {
      Bucket next = b->next;
      CE__free(b->value);
      CE__freeHash128(b->hash);
      CE__free(b);
      b = next;
    }
  }
  CE__free(set->buckets);
  CE__free(set);
}

int resize_hashset(CE__HashSet self, size_t newsize) {
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

Bucket newBucket_HashSet(CE__Hash128 hash, void* value, Bucket next) {
  Bucket ret = (Bucket)CE__malloc(sizeof(*ret));
  guard(!ret, nullptr);

  *ret = (struct Bucket) {
    .hash = hash,
    .value = value,
    .next = next
  };

  return ret;
}

int CE__insertHashSet(CE__HashSet self, void* value) {
  guard(self == nullptr, VALUE_IS_NULL);
  guard(value == nullptr, OTHER_VALUE_IS_NULL);
  
  CE__Hash128 hash = CE__hash128(value, self->element_size);
  size_t index = CE__Hash128Modulo(hash, self->capacity);

  Bucket* bucket = &self->buckets[index];

  loop {
    if (*bucket == nullptr) {
      void* k = CE__memdup(value, self->element_size);
      guard(k == nullptr, CANNOT_ALLOCATE);      

      *bucket = newBucket_HashSet(hash, k, nullptr);
      if (*bucket == nullptr) {
        CE__free(k);
        return CANNOT_ALLOCATE;
      }

      self->length++;
      return OK;
    }

    if (CE__Hash128Equals((*bucket)->hash, hash) && self->equals(value, (*bucket)->value, self->element_size))
      return OK;

    bucket = &((*bucket)->next);
  }

  if (((double)self->length  * sizeof(Bucket) / self->capacity) > 0.7) {
    size_t newsize = self->capacity * 2;
    guard(resize_hashset(self, newsize) != OK, CANNOT_ALLOCATE);
  }

  return OK;
}

bool CE__containsHashSet(CE__HashSet self, void* value) {
  guard(self == nullptr, nullptr);
  guard(value == nullptr, nullptr);

  CE__Hash128 hash = CE__hash128(value, self->element_size);
  size_t index = CE__Hash128Modulo(hash, self->capacity);

  Bucket* bucket = &self->buckets[index];

  loop {
    if (*bucket == nullptr)
      return false;

    if (CE__Hash128Equals((*bucket)->hash, hash) && self->equals(value, (*bucket)->value, self->element_size))
      return true;

    bucket = &((*bucket)->next);
  }
  return false;
}

int CE__removeHashSet(CE__HashSet self, void* value) {
  guard(self == nullptr, VALUE_IS_NULL);
  guard(value == nullptr, OTHER_VALUE_IS_NULL);
  
  CE__Hash128 hash = CE__hash128(value, self->element_size);
  size_t index = CE__Hash128Modulo(hash, self->capacity);

  Bucket* bucket = &self->buckets[index];

  loop {
    if (*bucket == nullptr) {
      CE__freeHash128(hash);
      return NOT_FOUND;
    }
    Bucket b = *bucket;

    if (CE__Hash128Equals(b->hash, hash) && self->equals(value, b->value, self->element_size)) {
      *bucket = b->next;
      CE__freeHash128(b->hash);
      CE__freeHash128(hash);
      CE__free(b->value);
      CE__free(b);
      self->length--;

      if (((double)self->length * sizeof(Bucket) / self->capacity) <= 0.33) {
        size_t newsize = getcap(self->length);
        if (newsize != self->capacity)
          guard(resize_hashset(self, newsize) != OK, CANNOT_ALLOCATE);
      }

      return OK;
    }

    bucket = &(b->next);
  }

  CE__freeHash128(hash);
  return OK;
}

int CE__reserveHashSet(CE__HashSet self, size_t amount) {
  size_t newsize = getcap(amount);
  return resize_hashset(self, newsize);
}
