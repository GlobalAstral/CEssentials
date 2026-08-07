#include <CEssentials.h>
#include <time.h>
#include <stdint.h>

static unsigned long long state = 0x4d595df4d0f33173;
static const unsigned long long multiplier = 6364136223846793005u;
static const unsigned long long increment = 1442695040888963407u;

static unsigned int rotr32(unsigned int x, unsigned r) {
	return x >> r | x << (-r & 31);
}

unsigned int pcg32(void) {
	unsigned long long x = state;
	unsigned count = (unsigned)(x >> 59);
	state = x * multiplier + increment;
	x ^= x >> 18;
	return rotr32((unsigned int)(x >> 27), count);
}

void pcg32_init(unsigned long long seed) {
	state = seed + increment;
	(void)pcg32();
}

void CE__randomSeed(unsigned long long seed) {
  pcg32_init(seed);
}

void CE__randomSeedTime() {
  CE__randomSeed(time(nullptr));
}

unsigned int CE__randomUInt32() {
  return pcg32();
}

unsigned long long CE__randomUInt64() {
  unsigned int low = CE__randomUInt32();
  unsigned int high = CE__randomUInt32();
  unsigned long long ret = (high << 31) | low;
  return ret;
}

int CE__randomInt(int min, int max) {
  unsigned int i = CE__randomUInt32();
  return min + (i % (max - min + 1));
}

size_t CE__randomSize(size_t min, size_t max) {
  unsigned long long l = CE__randomUInt64();
  return min + (l % (max - min + 1));
}

float CE__randomFloat() {
  float i = CE__randomUInt32();
  return i / ((float)UINT32_MAX + 1.0f);
}

double CE__randomDouble() {
  unsigned long long l = CE__randomUInt64();
  return l / ((double) UINT64_MAX + 1.0);
}

bool CE__randomBool() {
  return (CE__randomUInt32() % 2) == 0;
}

void CE__randomBytes(void* buffer, size_t size) {
  unsigned char* ptr = buffer;

  while (size >= sizeof(unsigned int)) {
    unsigned int n = CE__randomUInt32();
    memcpy(ptr, &n, sizeof(n));

    ptr += sizeof(n);
    size -= sizeof(n);
  }

  if (size > 0) {
    unsigned int n = CE__randomUInt32();
    memcpy(ptr, &n, size);
  }
}

size_t CE__randomIndex(size_t size) {
  return CE__randomSize(0, size - 1);
}
